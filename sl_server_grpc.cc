#include "sl_server_grpc.h"

#include <iostream>

grpc::Status ServiceLayerServer::registeruser(grpc::ServerContext* context, const chirp::RegisterRequest* request, chirp::RegisterReply* reply) {
  std::string username = request->username();
  slbe_.RegisterUser(username);
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServer::chirp(grpc::ServerContext* context, const chirp::ChirpRequest* request, chirp::ChirpReply* reply) {
  std::string username = request->username();
  std::string text = request->text();
  std::string parent_id = request->parent_id();
  slbe_.Chirp(username, text, parent_id);
  std::cout << "this chirp w user: " << username << " text: " << text << std::endl;
  int hi = 0;
  std::string s = "plzwork";
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServer::follow(grpc::ServerContext* context, const chirp::FollowRequest* request, chirp::FollowReply* reply) {
  std::string username = request->username();
  std::string to_follow = request->to_follow();
  slbe_.Follow(username, to_follow);
  return grpc::Status::OK;
}
grpc::Status ServiceLayerServer::read(grpc::ServerContext* context, const chirp::ReadRequest* request, chirp::ReadReply* reply) {
  std::vector<chirp::Chirp> values = slbe_.Read(request->chirp_id());
  for(chirp::Chirp val : values) {
    chirp::Chirp* createdChirp = reply->add_chirps();
    createdChirp->set_username(val.username());
    createdChirp->set_text(val.text());
    createdChirp->set_id(val.id());
    createdChirp->set_parent_id(val.parent_id());
    std::cout << "at this pt im getting: " << val.username() << " " << val.text() << std::endl;
    // TODO: make it a real timestamp
    // createdChirp->set_timestamp(val.timestamp());
  }
  return grpc::Status::OK;
}
grpc::Status ServiceLayerServer::monitor(grpc::ServerContext* context, const chirp::MonitorRequest* request, grpc::ServerWriter<chirp::MonitorReply>* stream) {
  // TODO: figure out what goes in here???
  // use key value server as a reference
  chirp::MonitorReply reply;
  std::string username = request->username();
  chirp::Chirp ch = slbe_.Monitor(username);
  reply.set_allocated_chirp(&ch);
  chirp::MonitorReply& sendingReply = reply;
  stream->Write(sendingReply);
  
  return grpc::Status::OK;
}

void RunServer() {
  std::string server_address("localhost:50002");
  ServiceLayerServer service_layer;

  grpc::ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service_layer);
  // Finally assemble the server.
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
} 