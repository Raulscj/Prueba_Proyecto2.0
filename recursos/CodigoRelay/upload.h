extern ESP8266WebServer server;
void PaginaSimple() {
  server.sendHeader("Connection", "close");
  server.send(200, "text/html", Pagina);
}

void ActualizarPaso1() {
  server.sendHeader("Connection", "close");
  server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
  ESP.restart();
}

void ActualizarPaso2() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    Serial.setDebugOutput(true);
#if defined(ESP8266)
    WiFiUDP::stopAll();
#endif
    Serial.printf("Actualizanddo: %s\n", upload.filename.c_str());
    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    if (!Update.begin(maxSketchSpace)) {
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (Update.end(true)) {
      Serial.printf("Actualizacion Exitosa: %u\nReiniciando...\n", upload.totalSize);
    } else {
      Update.printError(Serial);
    }
    Serial.setDebugOutput(false);
  } else {
    Serial.printf("Problema con la Actualizacion (Talves problema con la coneccion); Estado=%d\n", upload.status);
  }
  yield();
}
