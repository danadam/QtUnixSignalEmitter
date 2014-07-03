#include "MyQtDebug.h"

#include <QCoreApplication>
#include <QTimer>
#include <signal.h>
#include "QtUnixSignalEmitter.h"

#include <QObject>
class Main : public QObject
{
    Q_OBJECT
public:
    Main(QObject * parent = NULL) : QObject(parent) { }
public slots:
    void main();

    void handleSignal(int sig)
    {
        TRACE("%1(): %2").arg(__func__).arg(sig);
    }
    void handleInt()
    {
        TRACE("%1()").arg(__func__);
        QTimer::singleShot(0, qApp, SLOT(quit()));
    }
    void handleTerm()
    {
        TRACE("%1()").arg(__func__);
    }
    void handleHup()
    {
        TRACE("%1()").arg(__func__);
    }
};

void Main::main()
{
    QtUnixSignalEmitter * pQtUnixSignalEmitter = QtUnixSignalEmitter::instance();

    QObject::connect(
            pQtUnixSignalEmitter, SIGNAL(sigSignal(int)),
            this, SLOT(handleSignal(int))
        );
    QObject::connect(
            pQtUnixSignalEmitter, SIGNAL(sigInt()),
            this, SLOT(handleInt())
        );
    QObject::connect(
            pQtUnixSignalEmitter, SIGNAL(sigTerm()),
            this, SLOT(handleTerm())
        );
    QObject::connect(
            pQtUnixSignalEmitter, SIGNAL(sigHup()),
            this, SLOT(handleHup())
        );
}

int main(int argc, char * argv[])
{
    Q_UNUSED(argc); Q_UNUSED(argv);
    QCoreApplication app(argc, argv);
    prepareQtDebug();

    QtUnixSignalEmitter * pQtUnixSignalEmitter = QtUnixSignalEmitter::instance();
    if (pQtUnixSignalEmitter->registerSignal(SIGHUP))
    {
        TRACE("registered SIGHUP");
    }
    if (pQtUnixSignalEmitter->registerSignal(SIGINT))
    {
        TRACE("registered SIGINT");
    }
    if (pQtUnixSignalEmitter->registerSignal(SIGTERM))
    {
        TRACE("registered SIGTERM");
    }

    Main * pMain = new Main();
    QTimer::singleShot(0, pMain, SLOT(main()));

    return app.exec();
}
#include "main.moc"
