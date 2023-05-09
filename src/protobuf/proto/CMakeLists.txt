find_package(Protobuf REQUIRED)

file(GLOB_RECURSE PROTO_FILES *.proto)

protobuf_generate_cpp(PROTO_SRC PROTO_HEADER ${PROTO_FILES})
add_library(vcpkg_demo_protobuf_proto OBJECT ${PROTO_HEADER} ${PROTO_SRC})
target_link_libraries(
        vcpkg_demo_protobuf_proto PUBLIC
        protobuf::libprotobuf
        # protobuf::libprotobuf-lite
        # protobuf::libprotoc
)
