#include "client.h"
#include "threadpacketreceiver.h"
#include "lib/packetmanager.h"

const int Client::END = 0;
const int Client::ERROR_PARAM = 1;
const int Client::ERROR_THREAD = 2;
const int Client::ERROR_SERVER = 3;
const string Client::STR_CONFIG_ERROR = "Configuration was not loaded.";
const string Client::STR_ERROR_THREAD = "Error! Unable to run client.";
const string Client::STR_ERROR_SERVER = "Error! Unable to connect with server.";
const int Client::PROTOCOL_VERSION = 0;

bool Client::showScreen() {
  //muestra pantalla de configuracion
  ConnectScreen screen(_argc,_argv);
  if (!screen.connected()) {
    std::cerr << STR_CONFIG_ERROR << std::endl;
    return false;
  }
  //carga sonido
  _sound = screen.getSoundType();
  //teclas para moverse
  _movementKeys = screen.getMovementKeys();
  //mensajes predeterminados
  _chatShortcuts = screen.getShortcuts();
  //inicia conexion
  _socket = screen.getSocket();
  _playerName.assign(screen.getPlayerName());
  return true;
}

int Client::run() {
  if (!showScreen())
    return ERROR_PARAM;
  if (!_socket) {
    std::cerr << STR_ERROR_SERVER << std::endl;
    return ERROR_SERVER;
  }
  if (!*_socket) {
    std::cerr << STR_ERROR_SERVER << std::endl;
    delete _socket;
    return ERROR_SERVER;
  }
  //envia paquete connect
  PacketManager packetmanager(PROTOCOL_VERSION);
  Packet connect;
  packetmanager.createConnect(connect,_playerName);
  _socket->send(connect);
  //crea actualizador de graficos
  GraphicUpdater graphicUpdater(*_socket,_sound,_playerName,_movementKeys,_chatShortcuts,PROTOCOL_VERSION);
  //lanza threads
  ThreadPacketReceiver thReceiver(PROTOCOL_VERSION,*_socket,graphicUpdater);
  thReceiver.run();
  if (!thReceiver) {
    thReceiver.stop();
    thReceiver.join();
    std::cerr << STR_ERROR_THREAD << std::endl;
    delete _socket;
    return ERROR_THREAD;
  }
  graphicUpdater.startGame();
  //detiene hilos
  thReceiver.stop();
  thReceiver.join();
  delete _socket;
  return END;
}
