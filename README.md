QtUnixSignalEmitter
===================

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
