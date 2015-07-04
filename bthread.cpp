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

#include "bthread.h"
#include "boost/lambda/lambda.hpp"
#include "boost/lambda/bind.hpp"

BThread::BThread() 
{
    io_service = boost::shared_ptr< boost::asio::io_service >  ( new boost::asio::io_service);

    work = boost::shared_ptr< boost::asio::io_service::work >  (new boost::asio::io_service::work( *io_service ) ) ;

}

BThread::~BThread()
{

}

bool BThread::start()
{

        mThread =worker_threads.create_thread(
            boost::bind( &BThread::run , this, io_service )
            );

//    if (mThread == NULL)
//        {
//            mThread.reset(
//                new boost::thread(
//                    boost::lambda::bind(&BThread::run , this)));
//        }
//        else
//        {
//            throw std::runtime_error("multiple start");
//        }
}

void BThread::stop()
{
  io_service->stop();
}


boost::shared_ptr< boost::asio::io_service > BThread::getIoService()
{
  return io_service;
}

boost::shared_ptr< boost::asio::io_service::work > BThread::getWork()
{
  return work;
}

void BThread::setThreadName(std::string name)
{
  pthread_setname_np( mThread->native_handle(),name.c_str());
}


void BThread::join()
{
    worker_threads.join_all();
//  if (mThread)
//  {
//    mThread->join();
//  }
  
}
