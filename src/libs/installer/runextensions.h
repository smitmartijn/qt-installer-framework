/**************************************************************************
**
** Copyright (C) 2023 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Installer Framework.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
**************************************************************************/

#ifndef QTCONCURRENT_RUNEX_H
#define QTCONCURRENT_RUNEX_H

#include <qrunnable.h>
#include <qfutureinterface.h>
#include <qthreadpool.h>

QT_BEGIN_NAMESPACE

namespace QtConcurrent {

template <typename T,  typename FunctionPointer, typename... Args>
class StoredInterfaceFunctionCall : public QRunnable
{
public:
    StoredInterfaceFunctionCall(void (fn)(QFutureInterface<T> &, Args...), const Args...)
    : fn(fn), args(args) { }

    QFuture<T> start()
    {
        futureInterface.reportStarted();
        QFuture<T> future = futureInterface.future();
        QThreadPool::globalInstance()->start(this);
        return future;
    }

    void run() override
    {
        fn(futureInterface, std::forward<Args>(args)...);
        futureInterface.reportFinished();
    }
private:
    QFutureInterface<T> futureInterface;
    FunctionPointer fn;
    std::tuple<Args...> args;
};
template <typename T,  typename FunctionPointer, typename Class, typename... Args>
class StoredInterfaceMemberFunctionCall : public QRunnable
{
public:
    StoredInterfaceMemberFunctionCall(void (Class::*fn)(QFutureInterface<T> &, Args...), Class *object, const Args...)
    : fn(fn), object(object), args(args) { }

    QFuture<T> start()
    {
        futureInterface.reportStarted();
        QFuture<T> future = futureInterface.future();
        QThreadPool::globalInstance()->start(this);
        return future;
    }

    void run() override
    {
        (object->*fn)(futureInterface, std::forward<Args>(args)...);
        futureInterface.reportFinished();
    }
private:
    QFutureInterface<T> futureInterface;
    FunctionPointer fn;
    Class *object;
    std::tuple<Args...> args;
};

template <typename T, typename... Args>
QFuture<T> run(void (*functionPointer)(QFutureInterface<T> &, Args...), Args... args)
{
    return (new StoredInterfaceFunctionCall<T, void (*)(QFutureInterface<T> &, Args...), Args...>(functionPointer, args...))->start();
}

template <typename Class, typename T, typename... Args>
QFuture<T> run(void (Class::*fn)(QFutureInterface<T> &, Args...), Class *object, Args... args)
{
    return (new StoredInterfaceMemberFunctionCall<T, void (Class::*)(QFutureInterface<T> &, Args...), Class, Args...>(fn, object, args...))->start();
}

} // namespace QtConcurrent

QT_END_NAMESPACE

#endif // QTCONCURRENT_RUNEX_H
