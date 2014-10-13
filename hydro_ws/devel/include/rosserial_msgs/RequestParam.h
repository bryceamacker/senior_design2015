/* Software License Agreement (BSD License)
 *
 * Copyright (c) 2011, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of Willow Garage, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Auto-generated by gensrv_cpp from file /home/dexter/senior_design2015/hydro_ws/src/rosserial/rosserial_msgs/srv/RequestParam.srv
 *
 */


#ifndef ROSSERIAL_MSGS_MESSAGE_REQUESTPARAM_H
#define ROSSERIAL_MSGS_MESSAGE_REQUESTPARAM_H

#include <ros/service_traits.h>


#include <rosserial_msgs/RequestParamRequest.h>
#include <rosserial_msgs/RequestParamResponse.h>


namespace rosserial_msgs
{

struct RequestParam
{

typedef RequestParamRequest Request;
typedef RequestParamResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct RequestParam
} // namespace rosserial_msgs


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::rosserial_msgs::RequestParam > {
  static const char* value()
  {
    return "d7a0c2be00c9fd03cc69f2863de9c4d9";
  }

  static const char* value(const ::rosserial_msgs::RequestParam&) { return value(); }
};

template<>
struct DataType< ::rosserial_msgs::RequestParam > {
  static const char* value()
  {
    return "rosserial_msgs/RequestParam";
  }

  static const char* value(const ::rosserial_msgs::RequestParam&) { return value(); }
};


// service_traits::MD5Sum< ::rosserial_msgs::RequestParamRequest> should match 
// service_traits::MD5Sum< ::rosserial_msgs::RequestParam > 
template<>
struct MD5Sum< ::rosserial_msgs::RequestParamRequest>
{
  static const char* value()
  {
    return MD5Sum< ::rosserial_msgs::RequestParam >::value();
  }
  static const char* value(const ::rosserial_msgs::RequestParamRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::rosserial_msgs::RequestParamRequest> should match 
// service_traits::DataType< ::rosserial_msgs::RequestParam > 
template<>
struct DataType< ::rosserial_msgs::RequestParamRequest>
{
  static const char* value()
  {
    return DataType< ::rosserial_msgs::RequestParam >::value();
  }
  static const char* value(const ::rosserial_msgs::RequestParamRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::rosserial_msgs::RequestParamResponse> should match 
// service_traits::MD5Sum< ::rosserial_msgs::RequestParam > 
template<>
struct MD5Sum< ::rosserial_msgs::RequestParamResponse>
{
  static const char* value()
  {
    return MD5Sum< ::rosserial_msgs::RequestParam >::value();
  }
  static const char* value(const ::rosserial_msgs::RequestParamResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::rosserial_msgs::RequestParamResponse> should match 
// service_traits::DataType< ::rosserial_msgs::RequestParam > 
template<>
struct DataType< ::rosserial_msgs::RequestParamResponse>
{
  static const char* value()
  {
    return DataType< ::rosserial_msgs::RequestParam >::value();
  }
  static const char* value(const ::rosserial_msgs::RequestParamResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // ROSSERIAL_MSGS_MESSAGE_REQUESTPARAM_H
