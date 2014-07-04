QtUnixSignalEmitter
===================

Based on [Calling Qt Functions From Unix Signal Handlers](http://qt-project.org/doc/qt-4.8/unix-signals.html)

Public interface:

```C++
class QtUnixSignalEmitter : public QObject
{
public:
    static QtUnixSignalEmitter * instance();

    bool registerSignal(int sig, QString * pErrorMsg = 0);

signals:
    void sigSignal(int sig);
    void sigInt();
    void sigTerm();
    void sigHup();
};
```
