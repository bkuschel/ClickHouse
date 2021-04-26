# CMake in ClickHouse

## TL; DR How to make ClickHouse compile and link faster?

Developer only! This command will likely fulfill most of your needs. Run before calling `ninja`.

```cmake
cmake .. \
    -DCMAKE_C_COMPILER=/bin/clang-10 \
    -DCMAKE_CXX_COMPILER=/bin/clang++-10 \
    -DCMAKE_BUILD_TYPE=Debug \
    -DENABLE_CLICKHOUSE_ALL=OFF \
    -DENABLE_CLICKHOUSE_SERVER=ON \
    -DENABLE_CLICKHOUSE_CLIENT=ON \
    -DUSE_STATIC_LIBRARIES=OFF \
    -DSPLIT_SHARED_LIBRARIES=ON \
    -DENABLE_LIBRARIES=OFF \
    -DUSE_UNWIND=ON \
    -DENABLE_UTILS=OFF \
    -DENABLE_TESTS=OFF
```

## CMake files types

1. ClickHouse's source CMake files (located in the root directory and in `/src`).
2. Arch-dependent CMake files (located in `/cmake/*os_name*`).
3. Libraries finders (search for contrib libraries, located in `/cmake/find`).
3. Contrib build CMake files (used instead of libraries' own CMake files, located in `/cmake/modules`)

## List of CMake flags

* This list is auto-generated by [this Python script](https://github.com/clickhouse/clickhouse/blob/master/docs/tools/cmake_in_clickhouse_generator.py).
* The flag name is a link to its position in the code.
* If an option's default value is itself an option, it's also a link to its position in this list.
### ClickHouse modes

| Name | Default value | Description | Comment |
|------|---------------|-------------|---------|
| <a name="enable-clickhouse-all"></a>[`ENABLE_CLICKHOUSE_ALL`](https://github.com/clickhouse/clickhouse/blob/master/programs/CMakeLists.txt#L8) | `ON` | Enable all ClickHouse modes by default | The `clickhouse` binary is a multi purpose tool that contains multiple execution modes (client, server, etc.), each of them may be built and linked as a separate library. If you do not know what modes you need, turn this option OFF and enable SERVER and CLIENT only.  |
| <a name="enable-clickhouse-benchmark"></a>[`ENABLE_CLICKHOUSE_BENCHMARK`](https://github.com/clickhouse/clickhouse/blob/master/programs/CMakeLists.txt#L18) | `ENABLE_CLICKHOUSE_ALL` | Queries benchmarking mode | https://clickhouse.tech/docs/en/operations/utilities/clickhouse-benchmark/  |
| <a name="enable-clickhouse-client"></a>[`ENABLE_CLICKHOUSE_CLIENT`](https://github.com/clickhouse/clickhouse/blob/master/programs/CMakeLists.txt#L11) | `ENABLE_CLICKHOUSE_ALL` | Client mode (interactive tui/shell that connects to the server) |  |
| <a name="enable-clickhouse-compressor"></a>[`ENABLE_CLICKHOUSE_COMPRESSOR`](https://github.com/clickhouse/clickhouse/blob/master/programs/CMakeLists.txt#L23) | `ENABLE_CLICKHOUSE_ALL` | Data compressor and decompressor | https://clickhouse.tech/docs/en/operations/utilities/clickhouse-compressor/  |
| <a name="enable-clickhouse-copier"></a>[`ENABLE_CLICKHOUSE_COPIER`](https://github.com/clickhouse/clickhouse/blob/master/programs/CMakeLists.txt#L26) | `ENABLE_CLICKHOUSE_ALL` | Inter-cluster data copying mode | https://clickhouse.tech/docs/en/operations/utilities/clickhouse-copier/  |
| <a name="enable-clickhouse-extract-from-config"></a>[`ENABLE_CLICKHOUSE_EXTRACT_FROM_CONFIG`](https://github.com/clickhouse/clickhouse/blob/master/programs/CMakeLists.txt#L20) | `ENABLE_CLICKHOUSE_ALL` | Configs processor (extract values etc.) |  |
| <a name="enable-clickhouse-format"></a>[`ENABLE_CLICKHOUSE_FORMAT`](https://github.com/clickhouse/clickhouse/blob/master/programs/CMakeLists.txt#L28) | `ENABLE_CLICKHOUSE_ALL` | Queries pretty-printer and formatter with syntax highlighting |  |
| <a name="enable-clickhouse-git-import"></a>[`ENABLE_CLICKHOUSE_GIT_IMPORT`](https://github.com/clickhouse/clickhouse/blob/master/programs/CMakeLists.txt#L47) | `ENABLE_CLICKHOUSE_ALL` | A tool to analyze Git repositories | https://presentations.clickhouse.tech/matemarketing_2020/  |
| <a name="enable-clickhouse-install"></a>[`ENABLE_CLICKHOUSE_INSTALL`](https://github.com/clickhouse/clickhouse/blob/master/programs/CMakeLists.txt#L51) | `OFF` | Install ClickHouse without .deb/.rpm/.tgz packages (having the binary only) |  |
| <a name="enable-clickhouse-library-bridge"></a>[`ENABLE_CLICKHOUSE_LIBRARY_BRIDGE`](https://github.com/clickhouse/clickhouse/blob/master/programs/CMakeLists.txt#L43) | `ENABLE_CLICKHOUSE_ALL` | HTTP-server working like a proxy to Library dictionary source |  |
| <a name="enable-clickhouse-local"></a>[`ENABLE_CLICKHOUSE_LOCAL`](https://github.com/clickhouse/clickhouse/blob/master/programs/CMakeLists.txt#L15) | `ENABLE_CLICKHOUSE_ALL` | Local files fast processing mode | https://clickhouse.tech/docs/en/operations/utilities/clickhouse-local/  |
| <a name="enable-clickhouse-obfuscator"></a>[`ENABLE_CLICKHOUSE_OBFUSCATOR`](https://github.com/clickhouse/clickhouse/blob/master/programs/CMakeLists.txt#L32) | `ENABLE_CLICKHOUSE_ALL` | Table data obfuscator (convert real data to benchmark-ready one) | https://clickhouse.tech/docs/en/operations/utilities/clickhouse-obfuscator/  |
| <a name="enable-clickhouse-odbc-bridge"></a>[`ENABLE_CLICKHOUSE_ODBC_BRIDGE`](https://github.com/clickhouse/clickhouse/blob/master/programs/CMakeLists.txt#L37) | `ENABLE_CLICKHOUSE_ALL` | HTTP-server working like a proxy to ODBC driver |  |
| <a name="enable-clickhouse-server"></a>[`ENABLE_CLICKHOUSE_SERVER`](https://github.com/clickhouse/clickhouse/blob/master/programs/CMakeLists.txt#L10) | `ENABLE_CLICKHOUSE_ALL` | Server mode (main mode) |  |

### External libraries
Note that ClickHouse uses forks of these libraries, see https://github.com/ClickHouse-Extras.

| Name | Default value | Description | Comment |
|------|---------------|-------------|---------|
| <a name="enable-amqpcpp"></a>[`ENABLE_AMQPCPP`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/amqpcpp.cmake#L6) | `ENABLE_LIBRARIES` | Enalbe AMQP-CPP |  |
| <a name="enable-avro"></a>[`ENABLE_AVRO`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/avro.cmake#L2) | `ENABLE_LIBRARIES` | Enable Avro | Needed when using Apache Avro serialization format  |
| <a name="enable-base"></a>[`ENABLE_BASE64`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/base64.cmake#L2) | `ENABLE_LIBRARIES` | Enable base64 |  |
| <a name="enable-brotli"></a>[`ENABLE_BROTLI`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/brotli.cmake#L1) | `ENABLE_LIBRARIES` | Enable brotli |  |
| <a name="enable-capnp"></a>[`ENABLE_CAPNP`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/capnp.cmake#L1) | `ENABLE_LIBRARIES` | Enable Cap'n Proto |  |
| <a name="enable-cassandra"></a>[`ENABLE_CASSANDRA`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/cassandra.cmake#L6) | `ENABLE_LIBRARIES` | Enable Cassandra |  |
| <a name="enable-ccache"></a>[`ENABLE_CCACHE`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/ccache.cmake#L22) | `ENABLE_CCACHE_BY_DEFAULT` | Speedup re-compilations using ccache (external tool) | https://ccache.dev/  |
| <a name="enable-clang-tidy"></a>[`ENABLE_CLANG_TIDY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/analysis.cmake#L2) | `OFF` | Use clang-tidy static analyzer | https://clang.llvm.org/extra/clang-tidy/  |
| <a name="enable-curl"></a>[`ENABLE_CURL`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/curl.cmake#L1) | `ENABLE_LIBRARIES` | Enable curl |  |
| <a name="enable-datasketches"></a>[`ENABLE_DATASKETCHES`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/datasketches.cmake#L1) | `ENABLE_LIBRARIES` | Enable DataSketches |  |
| <a name="enable-embedded-compiler"></a>[`ENABLE_EMBEDDED_COMPILER`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/llvm.cmake#L5) | `ENABLE_LIBRARIES` | Set to TRUE to enable support for 'compile_expressions' option for query execution |  |
| <a name="enable-fastops"></a>[`ENABLE_FASTOPS`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/fastops.cmake#L2) | `ENABLE_LIBRARIES` | Enable fast vectorized mathematical functions library by Mikhail Parakhin |  |
| <a name="enable-gperf"></a>[`ENABLE_GPERF`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/gperf.cmake#L5) | `ENABLE_LIBRARIES` | Use gperf function hash generator tool |  |
| <a name="enable-grpc"></a>[`ENABLE_GRPC`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/grpc.cmake#L8) | `ENABLE_GRPC_DEFAULT` | Use gRPC |  |
| <a name="enable-gsasl-library"></a>[`ENABLE_GSASL_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/libgsasl.cmake#L1) | `ENABLE_LIBRARIES` | Enable gsasl library |  |
| <a name="enable-h"></a>[`ENABLE_H3`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/h3.cmake#L1) | `ENABLE_LIBRARIES` | Enable H3 |  |
| <a name="enable-hdfs"></a>[`ENABLE_HDFS`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/hdfs3.cmake#L2) | `ENABLE_LIBRARIES` | Enable HDFS |  |
| <a name="enable-icu"></a>[`ENABLE_ICU`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/icu.cmake#L2) | `ENABLE_LIBRARIES` | Enable ICU |  |
| <a name="enable-ldap"></a>[`ENABLE_LDAP`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/ldap.cmake#L5) | `ENABLE_LIBRARIES` | Enable LDAP |  |
| <a name="enable-libpqxx"></a>[`ENABLE_LIBPQXX`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/libpqxx.cmake#L1) | `ENABLE_LIBRARIES` | Enalbe libpqxx |  |
| <a name="enable-msgpack"></a>[`ENABLE_MSGPACK`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/msgpack.cmake#L1) | `ENABLE_LIBRARIES` | Enable msgpack library |  |
| <a name="enable-mysql"></a>[`ENABLE_MYSQL`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/mysqlclient.cmake#L2) | `ENABLE_LIBRARIES` | Enable MySQL |  |
| <a name="enable-nuraft"></a>[`ENABLE_NURAFT`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/nuraft.cmake#L1) | `ENABLE_LIBRARIES` | Enable NuRaft |  |
| <a name="enable-odbc"></a>[`ENABLE_ODBC`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/odbc.cmake#L1) | `ENABLE_LIBRARIES` | Enable ODBC library |  |
| <a name="enable-orc"></a>[`ENABLE_ORC`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/orc.cmake#L1) | `ENABLE_LIBRARIES` | Enable ORC |  |
| <a name="enable-parquet"></a>[`ENABLE_PARQUET`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/parquet.cmake#L2) | `ENABLE_LIBRARIES` | Enable parquet |  |
| <a name="enable-protobuf"></a>[`ENABLE_PROTOBUF`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/protobuf.cmake#L1) | `ENABLE_LIBRARIES` | Enable protobuf |  |
| <a name="enable-rapidjson"></a>[`ENABLE_RAPIDJSON`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/rapidjson.cmake#L1) | `ENABLE_LIBRARIES` | Use rapidjson |  |
| <a name="enable-rdkafka"></a>[`ENABLE_RDKAFKA`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/rdkafka.cmake#L1) | `ENABLE_LIBRARIES` | Enable kafka |  |
| <a name="enable-rocksdb"></a>[`ENABLE_ROCKSDB`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/rocksdb.cmake#L5) | `ENABLE_LIBRARIES` | Enable ROCKSDB |  |
| <a name="enable-s"></a>[`ENABLE_S3`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/s3.cmake#L2) | `ENABLE_LIBRARIES` | Enable S3 |  |
| <a name="enable-ssl"></a>[`ENABLE_SSL`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/ssl.cmake#L3) | `ENABLE_LIBRARIES` | Enable ssl | Needed when securely connecting to an external server, e.g. clickhouse-client --host ... --secure  |
| <a name="enable-stats"></a>[`ENABLE_STATS`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/stats.cmake#L1) | `ENABLE_LIBRARIES` | Enalbe StatsLib library |  |


### External libraries system/bundled mode

| Name | Default value | Description | Comment |
|------|---------------|-------------|---------|
| <a name="use-internal-avro-library"></a>[`USE_INTERNAL_AVRO_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/avro.cmake#L11) | `ON` | Set to FALSE to use system avro library instead of bundled |  |
| <a name="use-internal-aws-s-library"></a>[`USE_INTERNAL_AWS_S3_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/s3.cmake#L14) | `ON` | Set to FALSE to use system S3 instead of bundled (experimental set to OFF on your own risk) |  |
| <a name="use-internal-brotli-library"></a>[`USE_INTERNAL_BROTLI_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/brotli.cmake#L12) | `USE_STATIC_LIBRARIES` | Set to FALSE to use system libbrotli library instead of bundled | Many system ship only dynamic brotly libraries, so we back off to bundled by default  |
| <a name="use-internal-capnp-library"></a>[`USE_INTERNAL_CAPNP_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/capnp.cmake#L10) | `NOT_UNBUNDLED` | Set to FALSE to use system capnproto library instead of bundled |  |
| <a name="use-internal-curl"></a>[`USE_INTERNAL_CURL`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/curl.cmake#L10) | `NOT_UNBUNDLED` | Use internal curl library |  |
| <a name="use-internal-datasketches-library"></a>[`USE_INTERNAL_DATASKETCHES_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/datasketches.cmake#L5) | `NOT_UNBUNDLED` | Set to FALSE to use system DataSketches library instead of bundled |  |
| <a name="use-internal-grpc-library"></a>[`USE_INTERNAL_GRPC_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/grpc.cmake#L25) | `NOT_UNBUNDLED` | Set to FALSE to use system gRPC library instead of bundled. (Experimental. Set to OFF on your own risk) | Normally we use the internal gRPC framework. You can set USE_INTERNAL_GRPC_LIBRARY to OFF to force using the external gRPC framework, which should be installed in the system in this case. The external gRPC framework can be installed in the system by running sudo apt-get install libgrpc++-dev protobuf-compiler-grpc  |
| <a name="use-internal-gtest-library"></a>[`USE_INTERNAL_GTEST_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/gtest.cmake#L3) | `NOT_UNBUNDLED` | Set to FALSE to use system Google Test instead of bundled |  |
| <a name="use-internal-h-library"></a>[`USE_INTERNAL_H3_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/h3.cmake#L9) | `ON` | Set to FALSE to use system h3 library instead of bundled |  |
| <a name="use-internal-hdfs-library"></a>[`USE_INTERNAL_HDFS3_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/hdfs3.cmake#L14) | `ON` | Set to FALSE to use system HDFS3 instead of bundled (experimental - set to OFF on your own risk) |  |
| <a name="use-internal-icu-library"></a>[`USE_INTERNAL_ICU_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/icu.cmake#L15) | `NOT_UNBUNDLED` | Set to FALSE to use system ICU library instead of bundled |  |
| <a name="use-internal-ldap-library"></a>[`USE_INTERNAL_LDAP_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/ldap.cmake#L14) | `NOT_UNBUNDLED` | Set to FALSE to use system *LDAP library instead of bundled |  |
| <a name="use-internal-libcxx-library"></a>[`USE_INTERNAL_LIBCXX_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/cxx.cmake#L15) | `USE_INTERNAL_LIBCXX_LIBRARY_DEFAULT` | Disable to use system libcxx and libcxxabi libraries instead of bundled |  |
| <a name="use-internal-libgsasl-library"></a>[`USE_INTERNAL_LIBGSASL_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/libgsasl.cmake#L12) | `USE_STATIC_LIBRARIES` | Set to FALSE to use system libgsasl library instead of bundled | when USE_STATIC_LIBRARIES we usually need to pick up hell a lot of dependencies for libgsasl  |
| <a name="use-internal-libxml-library"></a>[`USE_INTERNAL_LIBXML2_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/libxml2.cmake#L1) | `NOT_UNBUNDLED` | Set to FALSE to use system libxml2 library instead of bundled |  |
| <a name="use-internal-llvm-library"></a>[`USE_INTERNAL_LLVM_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/llvm.cmake#L8) | `NOT_UNBUNDLED` | Use bundled or system LLVM library. |  |
| <a name="use-internal-msgpack-library"></a>[`USE_INTERNAL_MSGPACK_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/msgpack.cmake#L10) | `NOT_UNBUNDLED` | Set to FALSE to use system msgpack library instead of bundled |  |
| <a name="use-internal-mysql-library"></a>[`USE_INTERNAL_MYSQL_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/mysqlclient.cmake#L15) | `NOT_UNBUNDLED` | Set to FALSE to use system mysqlclient library instead of bundled |  |
| <a name="use-internal-odbc-library"></a>[`USE_INTERNAL_ODBC_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/odbc.cmake#L22) | `NOT_UNBUNDLED` | Use internal ODBC library |  |
| <a name="use-internal-orc-library"></a>[`USE_INTERNAL_ORC_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/orc.cmake#L11) | `ON` | Set to FALSE to use system ORC instead of bundled (experimental set to OFF on your own risk) |  |
| <a name="use-internal-parquet-library"></a>[`USE_INTERNAL_PARQUET_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/parquet.cmake#L16) | `NOT_UNBUNDLED` | Set to FALSE to use system parquet library instead of bundled |  |
| <a name="use-internal-poco-library"></a>[`USE_INTERNAL_POCO_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/poco.cmake#L1) | `ON` | Use internal Poco library |  |
| <a name="use-internal-protobuf-library"></a>[`USE_INTERNAL_PROTOBUF_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/protobuf.cmake#L14) | `NOT_UNBUNDLED` | Set to FALSE to use system protobuf instead of bundled. (Experimental. Set to OFF on your own risk) | Normally we use the internal protobuf library. You can set USE_INTERNAL_PROTOBUF_LIBRARY to OFF to force using the external protobuf library, which should be installed in the system in this case. The external protobuf library can be installed in the system by running sudo apt-get install libprotobuf-dev protobuf-compiler libprotoc-dev  |
| <a name="use-internal-rapidjson-library"></a>[`USE_INTERNAL_RAPIDJSON_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/rapidjson.cmake#L9) | `NOT_UNBUNDLED` | Set to FALSE to use system rapidjson library instead of bundled |  |
| <a name="use-internal-rdkafka-library"></a>[`USE_INTERNAL_RDKAFKA_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/rdkafka.cmake#L10) | `NOT_UNBUNDLED` | Set to FALSE to use system librdkafka instead of the bundled |  |
| <a name="use-internal-re-library"></a>[`USE_INTERNAL_RE2_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/re2.cmake#L1) | `NOT_UNBUNDLED` | Set to FALSE to use system re2 library instead of bundled [slower] |  |
| <a name="use-internal-rocksdb-library"></a>[`USE_INTERNAL_ROCKSDB_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/rocksdb.cmake#L14) | `NOT_UNBUNDLED` | Set to FALSE to use system ROCKSDB library instead of bundled |  |
| <a name="use-internal-snappy-library"></a>[`USE_INTERNAL_SNAPPY_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/snappy.cmake#L10) | `NOT_UNBUNDLED` | Set to FALSE to use system snappy library instead of bundled |  |
| <a name="use-internal-sparsehash-library"></a>[`USE_INTERNAL_SPARSEHASH_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/sparsehash.cmake#L1) | `ON` | Set to FALSE to use system sparsehash library instead of bundled |  |
| <a name="use-internal-ssl-library"></a>[`USE_INTERNAL_SSL_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/ssl.cmake#L12) | `NOT_UNBUNDLED` | Set to FALSE to use system *ssl library instead of bundled |  |
| <a name="use-internal-xz-library"></a>[`USE_INTERNAL_XZ_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/xz.cmake#L1) | `NOT_UNBUNDLED` | Set to OFF to use system xz (lzma) library instead of bundled |  |
| <a name="use-internal-zlib-library"></a>[`USE_INTERNAL_ZLIB_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/zlib.cmake#L1) | `NOT_UNBUNDLED` | Set to FALSE to use system zlib library instead of bundled |  |
| <a name="use-internal-zstd-library"></a>[`USE_INTERNAL_ZSTD_LIBRARY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/zstd.cmake#L1) | `NOT_UNBUNDLED` | Set to FALSE to use system zstd library instead of bundled |  |


### Other flags

| Name | Default value | Description | Comment |
|------|---------------|-------------|---------|
| <a name="add-gdb-index-for-gold"></a>[`ADD_GDB_INDEX_FOR_GOLD`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L209) | `OFF` | Add .gdb-index to resulting binaries for gold linker. | Ignored if `lld` is used  |
| <a name="arch-native"></a>[`ARCH_NATIVE`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L260) | `OFF` | Add -march=native compiler flag |  |
| <a name="clickhouse-split-binary"></a>[`CLICKHOUSE_SPLIT_BINARY`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L113) | `OFF` | Make several binaries (clickhouse-server, clickhouse-client etc.) instead of one bundled |  |
| <a name="compiler-pipe"></a>[`COMPILER_PIPE`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L249) | `ON` | -pipe compiler option | Less `/tmp` usage, more RAM usage.  |
| <a name="enable-check-heavy-builds"></a>[`ENABLE_CHECK_HEAVY_BUILDS`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L80) | `OFF` | Don't allow C++ translation units to compile too long or to take too much memory while compiling. | Take care to add prlimit in command line before ccache, or else ccache thinks that prlimit is compiler, and clang++ is its input file, and refuses to work  with multiple inputs, e.g in ccache log: [2021-03-31T18:06:32.655327 36900] Command line: /usr/bin/ccache prlimit --as=10000000000 --data=5000000000 --cpu=600 /usr/bin/clang++-11 - ...... std=gnu++2a -MD -MT src/CMakeFiles/dbms.dir/Storages/MergeTree/IMergeTreeDataPart.cpp.o -MF src/CMakeFiles/dbms.dir/Storages/MergeTree/IMergeTreeDataPart.cpp.o.d -o src/CMakeFiles/dbms.dir/Storages/MergeTree/IMergeTreeDataPart.cpp.o -c ../src/Storages/MergeTree/IMergeTreeDataPart.cpp  [2021-03-31T18:06:32.656704 36900] Multiple input files: /usr/bin/clang++-11 and ../src/Storages/MergeTree/IMergeTreeDataPart.cpp  Another way would be to use --ccache-skip option before clang++-11 to make ccache ignore it.  |
| <a name="enable-fuzzing"></a>[`ENABLE_FUZZING`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L130) | `OFF` | Fuzzy testing using libfuzzer | Implies `WITH_COVERAGE`  |
| <a name="enable-libraries"></a>[`ENABLE_LIBRARIES`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L383) | `ON` | Enable all external libraries by default | Turns on all external libs like s3, kafka, ODBC, ...  |
| <a name="enable-multitarget-code"></a>[`ENABLE_MULTITARGET_CODE`](https://github.com/clickhouse/clickhouse/blob/master/src/Functions/CMakeLists.txt#L102) | `ON` | Enable platform-dependent code | ClickHouse developers may use platform-dependent code under some macro (e.g. `ifdef ENABLE_MULTITARGET`). If turned ON, this option defines such macro. See `src/Functions/TargetSpecific.h`  |
| <a name="enable-tests"></a>[`ENABLE_TESTS`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L169) | `ON` | Provide unit_test_dbms target with Google.Test unit tests | If turned `ON`, assumes the user has either the system GTest library or the bundled one.  |
| <a name="enable-thinlto"></a>[`ENABLE_THINLTO`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L339) | `ON` | Clang-specific link time optimization | https://clang.llvm.org/docs/ThinLTO.html Applies to clang only. Disabled when building with tests or sanitizers.  |
| <a name="fail-on-unsupported-options-combination"></a>[`FAIL_ON_UNSUPPORTED_OPTIONS_COMBINATION`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L32) | `ON` | Stop/Fail CMake configuration if some ENABLE_XXX option is defined (either ON or OFF)   but is not possible to satisfy | If turned off: e.g. when ENABLE_FOO is ON, but FOO tool was not found, the CMake will continue.  |
| <a name="glibc-compatibility"></a>[`GLIBC_COMPATIBILITY`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L173) | `ON` | Enable compatibility with older glibc libraries. | Only for Linux, x86_64.  |
| <a name="linker-name"></a>[`LINKER_NAME`](https://github.com/clickhouse/clickhouse/blob/master/cmake/tools.cmake#L44) | `OFF` | Linker name or full path | Example values: `lld-10`, `gold`.  |
| <a name="llvm-has-rtti"></a>[`LLVM_HAS_RTTI`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/llvm.cmake#L40) | `ON` | Enable if LLVM was build with RTTI enabled |  |
| <a name="make-static-libraries"></a>[`MAKE_STATIC_LIBRARIES`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L106) | `USE_STATIC_LIBRARIES` | Disable to make shared libraries |  |
| <a name="parallel-compile-jobs"></a>[`PARALLEL_COMPILE_JOBS`](https://github.com/clickhouse/clickhouse/blob/master/cmake/limit_jobs.cmake#L10) | `""` | Maximum number of concurrent compilation jobs | 1 if not set  |
| <a name="parallel-link-jobs"></a>[`PARALLEL_LINK_JOBS`](https://github.com/clickhouse/clickhouse/blob/master/cmake/limit_jobs.cmake#L13) | `""` | Maximum number of concurrent link jobs | 1 if not set  |
| <a name="sanitize"></a>[`SANITIZE`](https://github.com/clickhouse/clickhouse/blob/master/cmake/sanitize.cmake#L7) | `""` | Enable one of the code sanitizers | Possible values: - `address` (ASan) - `memory` (MSan) - `thread` (TSan) - `undefined` (UBSan) - "" (no sanitizing)  |
| <a name="split-shared-libraries"></a>[`SPLIT_SHARED_LIBRARIES`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L111) | `OFF` | Keep all internal libraries as separate .so files | DEVELOPER ONLY. Faster linking if turned on.  |
| <a name="strip-debug-symbols-functions"></a>[`STRIP_DEBUG_SYMBOLS_FUNCTIONS`](https://github.com/clickhouse/clickhouse/blob/master/src/Functions/CMakeLists.txt#L51) | `STRIP_DSF_DEFAULT` | Do not generate debugger info for ClickHouse functions | Provides faster linking and lower binary size. Tradeoff is the inability to debug some source files with e.g. gdb (empty stack frames and no local variables)."  |
| <a name="unbundled"></a>[`UNBUNDLED`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L389) | `OFF` | Use system libraries instead of ones in contrib/ | We recommend avoiding this mode for production builds because we can't guarantee all needed libraries exist in your system. This mode exists for enthusiastic developers who are searching for trouble. Useful for maintainers of OS packages.  |
| <a name="use-include-what-you-use"></a>[`USE_INCLUDE_WHAT_YOU_USE`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L444) | `OFF` | Automatically reduce unneeded includes in source code (external tool) | https://github.com/include-what-you-use/include-what-you-use  |
| <a name="use-libcxx"></a>[`USE_LIBCXX`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/cxx.cmake#L1) | `NOT_UNBUNDLED` | Use libc++ and libc++abi instead of libstdc++ |  |
| <a name="use-sentry"></a>[`USE_SENTRY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/sentry.cmake#L13) | `ENABLE_LIBRARIES` | Use Sentry |  |
| <a name="use-simdjson"></a>[`USE_SIMDJSON`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/simdjson.cmake#L1) | `ENABLE_LIBRARIES` | Use simdjson |  |
| <a name="use-snappy"></a>[`USE_SNAPPY`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/snappy.cmake#L1) | `ENABLE_LIBRARIES` | Enable snappy library |  |
| <a name="use-static-libraries"></a>[`USE_STATIC_LIBRARIES`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L105) | `ON` | Disable to use shared libraries |  |
| <a name="use-unwind"></a>[`USE_UNWIND`](https://github.com/clickhouse/clickhouse/blob/master/cmake/find/unwind.cmake#L1) | `ENABLE_LIBRARIES` | Enable libunwind (better stacktraces) |  |
| <a name="werror"></a>[`WERROR`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L399) | `OFF` | Enable -Werror compiler option | Using system libs can cause a lot of warnings in includes (on macro expansion).  |
| <a name="weverything"></a>[`WEVERYTHING`](https://github.com/clickhouse/clickhouse/blob/master/cmake/warnings.cmake#L17) | `ON` | Enable -Weverything option with some exceptions. | Add some warnings that are not available even with -Wall -Wextra -Wpedantic. Intended for exploration of new compiler warnings that may be found useful. Applies to clang only  |
| <a name="with-coverage"></a>[`WITH_COVERAGE`](https://github.com/clickhouse/clickhouse/blob/master/CMakeLists.txt#L300) | `OFF` | Profile the resulting binary/binaries | Compiler-specific coverage flags e.g. -fcoverage-mapping for gcc  |

## Developer's guide for adding new CMake options

### Don't be obvious. Be informative.

Bad:
```cmake
option (ENABLE_TESTS "Enables testing" OFF)
```

This description is quite useless as is neither gives the viewer any additional information nor explains the option purpose.

Better:

```cmake
option(ENABLE_TESTS "Provide unit_test_dbms target with Google.test unit tests" OFF)
```

If the option's purpose can't be guessed by its name, or the purpose guess may be misleading, or option has some 
pre-conditions, leave a comment above the `option()` line and explain what it does. 
The best way would be linking the docs page (if it exists).
The comment is parsed into a separate column (see below).

Even better:

```cmake
# implies ${TESTS_ARE_ENABLED}
# see tests/CMakeLists.txt for implementation detail.
option(ENABLE_TESTS "Provide unit_test_dbms target with Google.test unit tests" OFF)
```

### If the option's state could produce unwanted (or unusual) result, explicitly warn the user.

Suppose you have an option that may strip debug symbols from the ClickHouse's part.
This can speed up the linking process, but produces a binary that cannot be debugged.
In that case, prefer explicitly raising a warning telling the developer that he may be doing something wrong. 
Also, such options should be disabled if applies.

Bad:
```cmake
option(STRIP_DEBUG_SYMBOLS_FUNCTIONS
    "Do not generate debugger info for ClickHouse functions.
    ${STRIP_DSF_DEFAULT})

if (STRIP_DEBUG_SYMBOLS_FUNCTIONS)
    target_compile_options(clickhouse_functions PRIVATE "-g0")
endif()

```
Better:

```cmake
# Provides faster linking and lower binary size.
# Tradeoff is the inability to debug some source files with e.g. gdb
# (empty stack frames and no local variables)."
option(STRIP_DEBUG_SYMBOLS_FUNCTIONS
    "Do not generate debugger info for ClickHouse functions."
    ${STRIP_DSF_DEFAULT})

if (STRIP_DEBUG_SYMBOLS_FUNCTIONS)
    message(WARNING "Not generating debugger info for ClickHouse functions")
    target_compile_options(clickhouse_functions PRIVATE "-g0")
endif()
```

### In the option's description, explain WHAT the option does rather than WHY it does something.

The WHY explanation should be placed in the comment.
You may find that the option's name is self-descriptive.

Bad:

```cmake
option(ENABLE_THINLTO "Enable Thin LTO. Only applicable for clang. It's also suppressed when building with tests or sanitizers." ON)
```

Better:

```cmake
# Only applicable for clang.
# Turned off when building with tests or sanitizers.
option(ENABLE_THINLTO "Clang-specific link time optimisation" ON).
```

### Don't assume other developers know as much as you do.

In ClickHouse, there are many tools used that an ordinary developer may not know. If you are in doubt, give a link to
the tool's docs. It won't take much of your time.

Bad:

```cmake
option(ENABLE_THINLTO "Enable Thin LTO. Only applicable for clang. It's also suppressed when building with tests or sanitizers." ON)
```

Better (combined with the above hint):

```cmake
# https://clang.llvm.org/docs/ThinLTO.html
# Only applicable for clang.
# Turned off when building with tests or sanitizers.
option(ENABLE_THINLTO "Clang-specific link time optimisation" ON).
```

Other example, bad:

```cmake
option (USE_INCLUDE_WHAT_YOU_USE "Use 'include-what-you-use' tool" OFF)
```

Better:

```cmake
# https://github.com/include-what-you-use/include-what-you-use
option (USE_INCLUDE_WHAT_YOU_USE "Reduce unneeded #include s (external tool)" OFF)
```

### Prefer consistent default values.

CMake allows you to pass a plethora of values representing boolean `true/false`, e.g. `1, ON, YES, ...`.
Prefer the `ON/OFF` values, if possible.