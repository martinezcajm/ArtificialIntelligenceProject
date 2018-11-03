/* Copyright 2017 ESAT. All rights reserved.    #####  #####      #     #####
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>  #      #         # #      #
 *                                              ###     ###     #   #     #
 * Networking communication                     #          #   #     #    #
 *                                              #####  #####  #       #   #
 */
#ifndef ESAT_NETWORK_H
#define ESAT_NETWORK_H 1

namespace ESAT {

  struct Message {
    const void *mem;
    size_t size;
  };

  struct NetworkAddress {
    const char *address;
    unsigned int port;
  };
  
  bool NetworkStart(const NetworkAddress &n = NetworkAddress{nullptr, 1234});

  void NetworkStop();

  // sends a message through the network, returns true on success. 
  bool NetworkSendMessage(const NetworkAddress &to,const Message &msg);

  // Message memory returned is available until NetworkRecvMessage is called again.
  // Returns true if msg represents a new message, false if there is no message 
  // waiting to be processed.
  // This method will never block the calling thread
  bool NetworkRecvMessage(Message *msg, NetworkAddress *from_optional = nullptr);

}

#endif