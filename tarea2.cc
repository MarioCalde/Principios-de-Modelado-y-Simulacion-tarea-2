//
// Maria Jose Monterrosa Mérida, 999014670
// Oscar Enrique Escalante Luna, 999013658
// Mario Rolando Calderón España, 999014184
//
//

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

/**
 * Let's make it more interesting by using several (n) `tic' modules,
 * and connecting every module to every other. For now, let's keep it
 * simple what they do: module 0 generates a message, and the others
 * keep tossing it around in random directions until it arrives at
 * module 2.
 */
class Txc2 : public cSimpleModule
{
  protected:
    virtual void forwardMessage(cMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Txc2);

void Txc2::initialize()
{
    EV << "me llamo " << getName() << "-"<< getIndex() <<"\n";
    EV << "tengo " << gateSize("out") << " puertas de salida y "<< gateSize("in") << " de entrada\n";

    if (getIndex() == 0) {
        // Boot the process scheduling the initial message as a self-message.
        char msgname[20];
        sprintf(msgname, "tic-%d", getIndex());
        cMessage *msg = new cMessage(msgname);
        scheduleAt(0.0, msg);
    }
}

void Txc2::handleMessage(cMessage *msg)
{
    if (getIndex() == 3) {
        // Message arrived.
        EV << "Message " << msg << " arrived.\n";
        delete msg;
    }
    else {
        // We need to forward the message.
        forwardMessage(msg);
    }
}

void Txc2::forwardMessage(cMessage *msg)
{
    // Obtener el nombre del módulo actual, la variable obtenida se guarda en getName
    const char *moduleName = getParentModule()->getName();

    // Si el módulo actual es tic0, tic2 o tic5, tratar el mensaje como un automensaje
    // en primer lugar se comprueba si los valores de tic0,tic2 y tic 5,strcmp es una función que compara
    //dos cadenas de caracteres, y devolverá 0 si son iguales. || este operador simboliza "o", y se evalua como
    //verdadero si son iguales.
    if (strcmp(moduleName, "tic0") == 0 || strcmp(moduleName, "tic2") == 0 || strcmp(moduleName, "tic5") == 0 || strcmp(moduleName, "tic3") == 0) {
        //EV es un objeto que se utiliza para enviar mensajes de registros
        //<< se utiliza para concatenar y enviar múltiples datos a un flujo de salida de manera secuencial.
        //si la condición anterior es verdadera, la siguiente línea maneja el mensaje como un automensaje
        EV << "El módulo " << moduleName << " trata el mensaje como un automensaje.\n";
        // en la variable n se guarda el número de la puerta de salida
        int n = gateSize("out");
        int k = intuniform(0, n-1);
        //se envia el mensaje por medio de la puerta seleccionada "k"
        send(msg, "out", k);
        return;
    }

    // Si el mensaje no llegó a través de una puerta (fue un automensaje), reenviarlo por un puerto aleatorio
    cGate *arrivalGate = msg->getArrivalGate();
    if (!arrivalGate) {
        EV << "El mensaje no llegó a través de una puerta. Reenviando por un puerto aleatorio.\n";
        int n = gateSize("out");
        int k = intuniform(0, n-1);
        EV << "Reenviando mensaje " << msg << " por puerto aleatorio out[" << k << "]\n";
        send(msg, "out", k);
        return;
    }

    // Obtener el índice del puerto por el que llegó el mensaje
    int arrivalGateIndex = arrivalGate->getIndex();

    // Reenviar el mensaje al siguiente nodo
    // Se calcula el índice de la siguiente puerta de salida a la que se reenviará el mensaje.
    int nextGateIndex = (arrivalGateIndex + 1) % gateSize("out");
    // : Esta línea imprime un mensaje en la consola de eventos (EV) indicando que el mensaje se está reenviando al siguiente nodo.
    EV << "Reenviando mensaje " << msg << " al puerto out[" << nextGateIndex << "]\n";
    // se envia el mensaje a través de la puerta de salida correspondiente al índice calculado (nextGateIndex)
    send(msg, "out", nextGateIndex);
}
