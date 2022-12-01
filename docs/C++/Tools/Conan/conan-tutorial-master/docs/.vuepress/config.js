module.exports = {
  base: '/conan-tutorial/',
  locales: {
    '/': {
      lang: 'zh-CN',
      title: '基于 Conan 的全平台全架构三方库管理',
      description: '云信 IM 团队在实践 Conan package manager 问题整理'
    }
  },
  themeConfig: {
    locales: {
      '/': {
          selectText: '选择语言',
          label: '简体中文',
          editLinkText: '在 GitHub 上编辑此页',
          serviceWorker: {
          updatePopup: {
            message: "发现新内容可用.",
            buttonText: "刷新"
          }
        },
        algolia: {},
        nav: [
          { text: '指南', link: '/guide/', ariaLabel: '指南' },
					{ text: 'Gitlab', link: 'https://g.hz.netease.com/nasdaq/im-cross-project/conan-tutorial' }
        ],
        sidebar: {
          '/guide/': [
            '',
            'problems',
            'practice',
            'cross-compilation',
            'advanced'
          ],
          '/resource/': []
        }
      }
    }
  },
}