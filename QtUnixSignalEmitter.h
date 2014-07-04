#ifndef QT_UNIX_SIGNAL_EMITTER_H
#define QT_UNIX_SIGNAL_EMITTER_H

#include <QObject>
#include <QScopedPointer>
#include <QSet>

class QSocketNotifier;
class QString;

class QtUnixSignalEmitter : public QObject
{
    Q_OBJECT
public:
    static QtUnixSignalEmitter * instance();

    bool registerSignal(int sig, QString * pErrorMsg = 0);

signals:
    void sigSignal(int sig);
    void sigInt();
    void sigTerm();
    void sigHup();

public:
    static void unix_signal_handler(int sig);
    ~QtUnixSignalEmitter();

private slots:
    void handleSignal();

private:
    static QScopedPointer<QtUnixSignalEmitter> s_pInstance;

    bool initializeFd(QString * pErrorMsg);
    bool initializeSignal(int sig, QString * pErrorMsg);

    QtUnixSignalEmitter();
    Q_DISABLE_COPY(QtUnixSignalEmitter)

    bool m_fdInitialized;
    QSet<int> m_signals;

    static int m_fd[2];
    QScopedPointer<QSocketNotifier> m_pSocketNotifier;
};

#endif

