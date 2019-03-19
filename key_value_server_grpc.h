#ifndef CPP_KEY_VALUE_SERVER_GRPC_H_
#define CPP_KEY_VALUE_SERVER_GRPC_H_

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "chirp_key_value.grpc.pb.h"
#include "key_value_backend.h"

// Key Value Server part of grpc that makes calls to the true key_value_backend 
class KeyValueServer final : public chirp::KeyValueStore::Service {
  public:
  	// function call to the lowest-level key-value backend's version of Put, which inserts into the map
    grpc::Status put(grpc::ServerContext* context, const chirp::PutRequest* request, chirp::PutReply* reply) override;
    // function call to the lowest-level key-value backend's version of Get, which retreives from the map
    grpc::Status get(grpc::ServerContext* context, grpc::ServerReaderWriter<chirp::GetReply, chirp::GetRequest>* stream) override;
    // function call to the lowest-level key-value backend's version of DeleteKey, which inserts into the map
    grpc::Status deletekey(grpc::ServerContext* context, const chirp::DeleteRequest* request, chirp::DeleteReply* reply) override;
  private:
  	// instance of the key-value backend class so that we can call it's functions
    KeyValueBackEnd key_value_back_end_;
};

#endif /*CPP_KEY_VALUE_SERVER_GRPC_H_*/