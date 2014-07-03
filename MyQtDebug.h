#ifndef MyQtDebug_h_GUARD
#define MyQtDebug_h_GUARD

#include <QtDebug>

#ifdef WIN32
#  define __func__ __FUNCTION__
#endif

#define TRACE(format) qDebug() << QString("%1(%2): %3").arg(__FILE__).arg(__LINE__).arg((format))
#define INFO(format) qWarning() << QString("%1(%2): %3").arg(__FILE__).arg(__LINE__).arg((format))
#define WARN(format) qCritical() << QString("%1(%2): %3").arg(__FILE__).arg(__LINE__).arg((format))
#define ERROR(format) qFatal() << QString("%1(%2): %3").arg(__FILE__).arg(__LINE__).arg((format))


#ifdef FULL_QT_HANDLER
#  define FUNC_ATTR
#else
#  define FUNC_ATTR inline
#endif

FUNC_ATTR void myQtMsgHandler(QtMsgType type, const char * msg);
FUNC_ATTR void prepareQtDebug() { qInstallMsgHandler(myQtMsgHandler); }

#undef FUNC_ATTR

//#define FULL_QT_HANDLER

#ifndef FULL_QT_HANDLER

#include <cstdio>
#include <cstring>
#include <string>

#include <QMutex>
#include <QMutexLocker>

void myQtMsgHandler(QtMsgType type, const char * msg)
{
    static QMutex mutex;

    // pass through Qt messages
    if (msg[0] != '\"')
    {
        fprintf(stderr, "%s\n", msg);
        return;
    }

    const int len = strlen(msg) - 3;  // 2 quotes and space at the end
    if (len <= 0)
        return;
    const char * msgNoQuotes = msg + 1;  // skip quote
    std::string output;
    output.reserve(10 + len);
    switch (type) {
        case QtDebugMsg:    output += "TRACE "; break;
        case QtWarningMsg:  output += "INFO "; break;
        case QtCriticalMsg: output += "WARN "; break;
        case QtFatalMsg:    output += "ERROR "; break;
        default:            output += "UNKNOWN "; break;
    }
    output.append(msgNoQuotes, len);

    QMutexLocker locker(&mutex);
    printf("%s\n", output.c_str());
}

#endif

#endif
