/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  mola <email>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef BTHREAD_H
#define BTHREAD_H

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/asio/io_service.hpp>

class BThread
{
public:
    BThread();
    ~BThread();
    
    bool start();
    void stop();
    
    void setThreadName(std::string name);
    
    boost::thread* getThread() { return mThread; };
    
    boost::shared_ptr< boost::asio::io_service > getIoService();
    boost::shared_ptr< boost::asio::io_service::work > getWork();
    
    void join();
private:
    virtual void run(boost::shared_ptr< boost::asio::io_service > io_service) = 0;

private:
    boost::thread_group worker_threads;
    boost::thread *mThread;

    boost::shared_ptr< boost::asio::io_service > io_service;
    boost::shared_ptr< boost::asio::io_service::work > work;

};

#endif // BTHREAD_H
