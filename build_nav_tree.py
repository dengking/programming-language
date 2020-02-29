import logging
import os
import pathlib

from yaml import load as yaml_load, dump as yaml_dump

try:
    from yaml import CLoader as Loader, CDumper as Dumper
except ImportError:
    from yaml import Loader, Dumper


class Node:
    def __init__(self):
        self.label = None
        self.children = None


class DictionaryTreeBuilder:
    def __init__(self, root_dir='docs'):
        self.root_dir = root_dir  # 根路径名称
        self.root_node = dict()  # 使用dict来作为节点
        self.build()
        self.yaml_nav = yaml_dump(self.root_node, default_flow_style=False)

    def build(self):
        """
        构造一棵树
        :return:
        """
        for dirpath, dirnames, filenames in os.walk(self.root_dir):
            """dirpath is a string, the path to the directory.  dirnames is a list of
            the names of the subdirectories in dirpath (excluding '.' and '..').
            filenames is a list of the names of the non-directory files in dirpath.
            Note that the names in the lists are just names, with no path components.
            To get a full path (which begins with top) to a file or directory in
            dirpath, do os.path.join(dirpath, name)."""
            if dirpath == self.root_dir:
                self.add_descend_node(self.root_node, '', dirnames, filenames)
            else:
                # 非第一级
                sub_dir_pathes = self.split_path(dirpath)
                current_node = self.root_node
                path = os.path.join(*sub_dir_pathes[1:])
                for sub_dir_path in sub_dir_pathes[1:]:
                    """沿着路径找到所属的节点"""

                    if sub_dir_path in current_node:
                        current_node = current_node[sub_dir_path]
                    else:
                        error = '{}未添加'.format(sub_dir_path)
                        raise Exception(error)
                self.add_descend_node(current_node, path, dirnames, filenames)

    def add_descend_node(self, node, dirpath, dirnames, filenames):
        """
        添加后裔节点
        :param node:
        :param dirnames:
        :param filenames:
        :return:
        """
        # 第一级
        for dirname in dirnames:
            key = self.key_name(dirname)
            node[key] = dict()  # 内节点
        for filename in filenames:
            if filename.endswith('.md'):
                key = self.key_name(filename[0:-3])
                node[key] = os.path.join(dirpath, filename)  # 叶子节点

    @staticmethod
    def key_name(dirname_or_filename):
        return '- ' + dirname_or_filename

    @staticmethod
    def split_path(path):
        path = os.path.normpath(path)
        return path.split(os.sep)


class NavBuilder:
    """
    每个目录下都有一个配置文件mkdocs.yml，根据配置文件中的内容来进行组装，最终的组装结果是一棵树，下面描述的是组装过程：
    - 如果值是一个文件，则是叶子节点
    - 如果值是一个目录，则是一个内节点，需要进行扩展

    显然这个过程是非常类似于top-down parsing

    从root_dir开始

    如下是通过yaml.load读入到内存中的mkdocs.yml的格式：
    {'nav': [{'Home': 'index.md'},
    {'Chapter1': 'Chapter1-Introduction'},
    {'Chapter2': 'Chapter2-A-Simple-Syntax-Directed-Translator'},
    {'Chapter3': 'Chapter3-Lexical-Analysis'},
    {'Chapter4': 'Chapter4-Syntax-Analysis'},
    {'Chapter5': 'Chapter5-Syntax-Directed-Translation'},
    {'Chapter6': 'Chapter6-Intermediate-Code-Generation'},
    {'Chapter7': 'Chapter7-Run-Time Environments'},
    {'Chapter9': 'Chapter9-Machine-Independent-Optimizations'}]}
    非常类似于前缀树

    """
    MkdocsTemplateFileName = 'mkdocs-template.yml'  # 模板文件
    MkdocsFileName = 'mkdocs.yml'
    Nav = 'nav'

    def __init__(self, root_dir='docs'):
        self.root_dir = root_dir  # 根路径名称
        self.root_nav_label = self.Nav
        # 最终结果就是一棵树
        # 它表示这棵树的root节点，
        # key作为节点的label(type hint str)，
        # value作为节点的子节点(type hint: list of dict)
        self.root_nav_node = dict()

    def build(self):
        """
        从根目录开始，逐步添加目录
        :return:
        """
        mkdocs_file_path = os.path.join(self.root_dir, self.MkdocsFileName)
        nav_path = self.root_nav_label
        self.__expand__(nav_path, mkdocs_file_path)
        self.__save__()

    def __expand__(self, nav_path, mkdocs_file_path):
        """
        - 采用深度优先来扩展non-terminal
        - 对于terminal，需要补全路径
        :param nav_path: 导航栏路径
        :param mkdocs_file_path: mkdocs.yml文件的路径
        :return:
        """
        if os.path.exists(mkdocs_file_path):
            child_nodes = self.__load__(mkdocs_file_path)
            self.__add_node__(nav_path, child_nodes)

            split_file_path = mkdocs_file_path.split(os.sep)
            print(split_file_path)
            for child_node in child_nodes:  # type hint: dict
                for child_node_label, child_node_value in child_node.items():
                    if child_node_value.endswith('.md'):
                        # 文件，相当于terminal，需要补全路径
                        __split_file_path = split_file_path[1:-1]  # mkdocs文件中的文件路径不包括docs，所以从1开始
                        if __split_file_path:
                            current_file_path = os.path.join(*__split_file_path)
                            child_node[child_node_label] = pathlib.Path(
                                os.path.join(current_file_path, child_node_value)).as_posix()  # 补全路径，使用POSIX格式路径
                        else:
                            child_node[child_node_label] = child_node_value
                    else:
                        # 目录，相当于non-terminal，需要进行扩展
                        current_file_path = os.path.join(*split_file_path[0:-1])
                        self.__expand__(os.path.join(nav_path, child_node_label),
                                        os.path.join(current_file_path, child_node_value, self.MkdocsFileName))
        else:
            log = "配置文件'{}'不存在".format(mkdocs_file_path)
            raise Exception(log)

    def __add_node__(self, nav_path, child_nodes):
        """

        :param nav_path: 导航路径，它所指向的一定是一个non-terminal
        :param child_nodes: type hint: list of dict
        :return:
        """

        def __find_in_nodes__(label, nodes):
            """

            :param label:
            :param nodes: list of dict
            :return:
            """
            for node in nodes:  # type hint: dict
                if label in node:
                    return node
            return None

        if nav_path == self.root_nav_label:
            self.root_nav_node[nav_path] = child_nodes
        else:
            split_nav_path = nav_path.split(os.sep)  # os.path.split(nav_path)
            root_nav = split_nav_path[0]
            if root_nav in self.root_nav_node:
                children = self.root_nav_node[root_nav]  # type hint: list of dict
                # 不断进行迭代，直到到达了该路径指向的节点
                for nav in split_nav_path[1:]:
                    node = __find_in_nodes__(nav, children)
                    if node:
                        children = node[nav]
                        if isinstance(children, list):
                            continue
                        elif isinstance(children, str):
                            if children.endswith('.md'):
                                log = "nav路径'{}'指向的是单个文件，无需进行扩展".format(nav_path)
                                raise Exception(log)
                            else:
                                log = "nav路径'{}'指向的是一个目录，需要进行扩展，现进行扩展".format(nav_path)
                                logging.debug(log)
                                node[nav] = child_nodes
                                return
                        else:
                            log = "不支持的类型'{}'".format(type(children))
                            raise Exception(log)
                    else:
                        log = "'{}'还未添加到树中".format(nav)
                        raise Exception(log)
            else:
                log = "'{}'还未添加到树中".format(root_nav)
                raise Exception(log)

    def __save__(self):
        with open(self.MkdocsTemplateFileName, encoding='utf-8') as template_f, open(self.MkdocsFileName, 'w',
                                                                                     encoding='utf-8') as f:
            mkdocs = yaml_load(template_f, Loader=Loader)
            mkdocs[self.Nav] = self.root_nav_node[self.Nav]
            yaml_dump(mkdocs, f, default_flow_style=False)

    def __load__(self, mkdocs_file_path):
        """

        :param mkdocs_file_path: mkdocs.yml文件的路径
        :return: type hint: list of dict
        """
        with open(mkdocs_file_path, encoding='utf-8') as f:
            nav = yaml_load(f, Loader=Loader)  # type hint list of dict
            if self.Nav in nav:
                child_nodes = nav[self.Nav]  # type hint list of dict
                return child_nodes
            else:
                log = "文件'{}'中没有nav".format(mkdocs_file_path)
                raise Exception(log)

