/**************************************************************************
**
** This file is part of Qt SDK**
**
** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).*
**
** Contact:  Nokia Corporation qt-info@nokia.com**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception version
** 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you are unsure which license is appropriate for your use, please contact
** (qt-info@nokia.com).
**
**************************************************************************/
#ifndef FSENGINECLIENT_H
#define FSENGINECLIENT_H

#include <QtCore/QAbstractFileEngineHandler>
#include <QtCore/QSettings>

#ifdef FSENGINE_TCP
#include <QtNetwork/QHostAddress>
QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE
#else
QT_BEGIN_NAMESPACE
class QLocalSocket;
QT_END_NAMESPACE
#endif

#include "installer_global.h"

class INSTALLER_EXPORT FSEngineClientHandler : public QAbstractFileEngineHandler
{
public:
    FSEngineClientHandler();
#ifdef FSENGINE_TCP
    FSEngineClientHandler( quint16 port, const QHostAddress& a = QHostAddress::LocalHost );
    void init( quint16 port, const QHostAddress& a = QHostAddress::LocalHost );

#else
    FSEngineClientHandler( const QString& socket );
    void init( const QString& socket );
#endif
    ~FSEngineClientHandler();

    static FSEngineClientHandler* instance();

    QAbstractFileEngine* create( const QString& fileName ) const;

    void enableTestMode();
    void setActive( bool active );
    bool isActive() const;
    bool isServerRunning() const;

    QString authorizationKey() const;

    void setStartServerCommand( const QString& command, bool startAsAdmin = false );
    void setStartServerCommand( const QString& command, const QStringList& arguments, bool startAsAdmin = false );

#ifdef FSENGINE_TCP
    bool connect( QTcpSocket* socket );
#else
    bool connect( QLocalSocket* socket );
#endif

private:
    class Private;
    Private* d;
};

class INSTALLER_EXPORT QSettingsWrapper : public QObject
{
    Q_OBJECT
public:
    enum Format
    {
        NativeFormat,
        IniFormat,
        InvalidFormat
    };

    enum Status
    {
        NoError,
        AccessError,
        FormatError
    };

    enum Scope
    {
        UserScope,
        SystemScope
    };

    explicit QSettingsWrapper( const QString& organization, const QString& application = QString(), QObject* parent = 0 );
    QSettingsWrapper( QSettingsWrapper::Scope scope, const QString& organization, const QString& application = QString(), QObject* parent = 0 );
    QSettingsWrapper( QSettingsWrapper::Format format, QSettingsWrapper::Scope scope, const QString& organization, const QString& application = QString(), QObject* parent = 0 );
    QSettingsWrapper( const QString& fileName, QSettingsWrapper::Format format, QObject* parent = 0 );
    explicit QSettingsWrapper( QObject* parent = 0 );
    ~QSettingsWrapper();

    QStringList allKeys() const;
    QString applicationName() const;
    void beginGroup( const QString& prefix );
    int beginReadArray( const QString& prefix );
    void beginWriteArray( const QString& prefix, int size = -1 );
    QStringList childGroups() const;
    QStringList childKeys() const;
    void clear();
    bool contains( const QString& key ) const;
    void endArray();
    void endGroup();
    bool fallbacksEnabled() const;
    QString fileName() const;
    QSettingsWrapper::Format format() const;
    QString group() const;
    QTextCodec* iniCodec() const;
    bool isWritable() const;
    QString organizationName() const;
    void remove( const QString& key );
    QSettingsWrapper::Scope scope() const;
    void setArrayIndex( int i );
    void setFallbacksEnabled( bool b );
    void setIniCodec( QTextCodec* codec );
    void setIniCodec( const char* codecName );
    void setValue( const QString& key, const QVariant& value );
    QSettingsWrapper::Status status() const;
    void sync();
    QVariant value( const QString& key, const QVariant& defaultValue = QVariant() ) const;

private:
    class Private;
    Private* d;
};

#define QSettings QSettingsWrapper

#endif
