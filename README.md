# pokemon-console

### dependencias

- cmake >= 3.10
- [rpclib](https://github.com/rpclib/rpclib)
- nlohmann-json3-dev package(ubuntu)

### como buildar

- Compile a rpclib em seu computador, instruções estão no próprio repositório.
- altere os arquivos client/CMakeLists.txt e server/CMakeLists.txt com o caminho da biblioteca em seu computador

> set(RPCLIB_INCLUDE_DIR "$ENV{HOME}/rpclib/include")
> set(RPCLIB_LIBRARY_DIR "$ENV{HOME}/rpclib/build")

- cd server && mkdir build && cd build && cmake .. && cmake --build . 
- cd client && mkdir build && cd build && cmake .. && cmake --build . 

> após a primeira vez, compile apenas entrando na pasta build e executando cmake --build .

### como jogar

- Abra uma instância do terminal separado e execute o servidor, será solicitado a porta
- Execute os clientes informando a porta na qual o servidor está executando

> os executaveis precisam ser executados de dentro da pasta /build/ para poder pegar os recursos do lugar certo

> os executaveis do ultimo build estão na pasta raiz, jogue-os nas pastas corretas e não precisará compilar manualmente(funcionará em linux ubuntu)