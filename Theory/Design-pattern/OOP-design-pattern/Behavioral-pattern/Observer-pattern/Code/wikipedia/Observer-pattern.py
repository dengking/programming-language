class Observable(object):
    def __init__(self) -> None:
        self._observers = []

    def register_observer(self, observer) -> None:
        self._observers.append(observer)

    def notify_observers(self, *args, **kwargs) -> None:
        for observer in self._observers:
            observer.notify(self, *args, **kwargs)


class Observer(object):
    def __init__(self, observable) -> None:
        observable.register_observer(self)

    def notify(self, observable, *args, **kwargs) -> None:
        print('Got', args, kwargs, 'From', observable)


subject = Observable()
observer = Observer(subject)
subject.notify_observers('test')
