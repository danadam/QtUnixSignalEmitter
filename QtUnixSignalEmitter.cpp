#include "QtUnixSignalEmitter.h"

#include <QString>
#include <QSocketNotifier>

#include <errno.h>
#include <string.h>

#include <unistd.h>

#include <sys/socket.h>
#include <signal.h>

namespace
{

int l_signalFd[2];

void unix_signal_handler(int sig)
{
    ::write(l_signalFd[0], &sig, sizeof(sig));
}

} // namespace

QScopedPointer<QtUnixSignalEmitter> QtUnixSignalEmitter::s_pInstance;

//static
QtUnixSignalEmitter * QtUnixSignalEmitter::instance()
{
    if (!s_pInstance)
        s_pInstance.reset(new QtUnixSignalEmitter());

    return s_pInstance.data();
}

QtUnixSignalEmitter::QtUnixSignalEmitter()
    : m_fdInitialized(false)
{

}

QtUnixSignalEmitter::~QtUnixSignalEmitter()
{

}

bool QtUnixSignalEmitter::registerSignal(int sig, QString * pErrorMsg /*= 0*/)
{
    if (m_signals.contains(sig))
        return true;

    if (!m_fdInitialized)
    {
        if (!initializeFd(pErrorMsg))
            return false;

        m_fdInitialized = true;
    }

    return initializeSignal(sig, pErrorMsg);
}

bool QtUnixSignalEmitter::initializeFd(QString * pErrorMsg)
{
    int rc = ::socketpair(AF_UNIX, SOCK_STREAM, 0, l_signalFd);
    if (rc != 0)
    {
        if (pErrorMsg)
        {
            int errsv = errno;
            *pErrorMsg = QString("socketpair() failed: %1").arg(strerror(errsv));
        }
        return false;
    }

    m_pSocketNotifier.reset(new QSocketNotifier(l_signalFd[1], QSocketNotifier::Read));
    QObject::connect(
            m_pSocketNotifier.data(), SIGNAL(activated(int)),
            this, SLOT(handleSignal())
        );

    return true;
}

bool QtUnixSignalEmitter::initializeSignal(int sig, QString * pErrorMsg)
{
    struct sigaction action;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);

    action.sa_handler = unix_signal_handler;

    int rc = sigaction(sig, &action, 0);
    if (rc != 0)
    {
        if (pErrorMsg)
        {
            int errsv = errno;
            *pErrorMsg = QString("sigaction() failed: %1").arg(strerror(errsv));
        }
        return false;
    }

    m_signals.insert(sig);
    return true;
}

void QtUnixSignalEmitter::handleSignal()
{
    m_pSocketNotifier->setEnabled(false);

    int sig = 0;
    ::read(l_signalFd[1], &sig, sizeof(sig));

    emit sigSignal(sig);
    switch (sig)
    {
    case SIGTERM:
        emit sigTerm(); break;
    case SIGINT:
        emit sigInt(); break;
    case SIGHUP:
        emit sigHup(); break;
    default:
        break;
    }

    m_pSocketNotifier->setEnabled(true);
}
