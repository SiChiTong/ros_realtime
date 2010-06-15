/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2010, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Willow Garage nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

#include <ros/rt/detail/publisher_manager.h>
#include <ros/rt/detail/subscriber_manager.h>
#include <ros/rt/detail/simple_gc.h>

#include <boost/thread.hpp>

namespace rosrt
{

namespace detail
{

class Managers
{
public:
  Managers(const InitOptions& ops);
  ~Managers();

  PublisherManager* getPublisherManager() { return pub_manager_; }
  SubscriberManager* getSubscriberManager() { return sub_manager_; }
  SimpleGC* getGC() { return gc_; }

private:

  PublisherManager* pub_manager_;
  SubscriberManager* sub_manager_;
  SimpleGC* gc_;
};
typedef boost::shared_ptr<Managers> ManagersPtr;

static ManagersPtr g_managers;

Managers::Managers(const InitOptions& ops)
{
  pub_manager_ = new PublisherManager(ops);
  sub_manager_ = new SubscriberManager;
  gc_ = new SimpleGC(ops);
}

Managers::~Managers()
{
  delete pub_manager_;
  delete sub_manager_;
  delete gc_;
}

PublisherManager* getPublisherManager()
{
  ROS_ASSERT(g_managers);
  return g_managers->getPublisherManager();
}

SubscriberManager* getSubscriberManager()
{
  ROS_ASSERT(g_managers);
  return g_managers->getSubscriberManager();
}

SimpleGC* getGC()
{
  ROS_ASSERT(g_managers);
  return g_managers->getGC();
}

} // namespace detail

static bool g_initialized = false;

void init(const InitOptions& ops)
{
  if (!g_initialized)
  {
    ROS_ASSERT(!detail::g_managers);
    detail::g_managers.reset(new detail::Managers(ops));

    g_initialized = true;
  }
}

void shutdown()
{
  detail::g_managers.reset();
}

} // namespace rosrt


