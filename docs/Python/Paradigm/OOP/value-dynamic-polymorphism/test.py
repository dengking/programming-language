os_info = {
    'mac': 1,
    'windows': 2,
    'ios': 3,
}


class Base:
    os = None

    def __init__(self):
        self.os_id = os_info[self.os]


class Windows(Base):
    os = 'windows'

    def __init__(self):
        Base.__init__(self)


class Mac(Base):
    os = 'mac'

    def __init__(self):
        Base.__init__(self)


class IOS(Base):
    os = 'ios'

    def __init__(self):
        Base.__init__(self)


if __name__ == '__main__':
    windows = Windows()
    print(windows.os_id)
    mac = Mac()
    print(mac.os_id)
    ios = IOS()
    print(ios.os_id)
