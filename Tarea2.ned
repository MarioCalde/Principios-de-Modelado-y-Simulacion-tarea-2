
//
// Maria Jose Monterrosa Mérida, 999014670
//Oscar Enrique Escalante Luna, 999013658
// Mario Rolando Calderón España, 999014184
//


simple Txc2
{
    parameters:
        @display("i=block/routing");
    gates:
        input in[];  // declare in[] and out[] to be vector gates
        output out[];
}

network Tarea2
{
    submodules:
        tic[6]: Txc2;
    connections:
        tic[0].out++ --> {  delay = 100ms; } --> tic[1].in++;
        tic[0].in++ <-- {  delay = 100ms; } <-- tic[1].out++;

        tic[1].out++ --> {  delay = 100ms; } --> tic[2].in++;
        tic[1].in++ <-- {  delay = 100ms; } <-- tic[2].out++;

        tic[1].out++ --> {  delay = 100ms; } --> tic[4].in++;
        tic[1].in++ <-- {  delay = 100ms; } <-- tic[4].out++;

        tic[3].out++ --> {  delay = 100ms; } --> tic[4].in++;
        tic[3].in++ <-- {  delay = 100ms; } <-- tic[4].out++;

        tic[4].out++ --> {  delay = 100ms; } --> tic[5].in++;
        tic[4].in++ <-- {  delay = 100ms; } <-- tic[5].out++;
}

