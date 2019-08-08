
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //Iniciação do LCD
int vezes=2, seg=0, min=0,hor=0, x=0, y=0, z=0, proxH=0, cont;
int r=0, seg_agua5=0;
int seg_agua=0;
int direcao=0;
int passos_motor = 1;
int Cursor=0;
int Linha=0;
int A=0;
int pino_passo_racao = 3, // Pino Passos motor ração
pino_direcao_racao = 2; // Pino Direção motor ração
int pino_passo_agua = 11, // Pino Passos motor água
pino_direcao_agua = 10; // Pino Direção motor água
int sensor = 12; // Pino Sensor de nível
int set[] = { hor, min, seg }; // Parâmetros relógio
int hora_racao[3]; // Vetor Horas para ração
int minuto_racao[3]; // Vetor Minutos para ração
int seg_racao[3]; // Vetor Segundos para Ração
int thisTime = 0; // Posição do Cursor
void setup()
{
 Serial.begin(9600);
 pinMode(pino_passo_racao, OUTPUT);
 pinMode(pino_direcao_racao, OUTPUT);
 pinMode(pino_passo_agua, OUTPUT);
 pinMode(pino_direcao_agua, OUTPUT);
 pinMode(sensor, INPUT);

inicializar(); // Mensagem para começo do programa
 numVEZES(); // Ajustar entre 2 ou 3 vezes ao dia
 tempo_racao(); // Ajustar os horários do funcionamento do motor da ração 

}
void loop()
{
 if (x==0) {
// x==0 indica que o botão select ainda não foi pressionado para confirmação da hora
//atual, quando o usuário apertar select, x=1, nao realizando mais este passo.
 lcd.setCursor(0,0);
 lcd.print("Ajuste a hora");
 lcd.setCursor(0,1);
 lcd.print("atual:");
 Cursor=7;
 Linha=1;
 ajusteRELOGIO();
// Função que reconhece qual botão foi pressionado para o ajuste do relogio
 //piscar();
// Pisca os digitos onde o cursor está selecionado
 }
 else{
 while (z==0) {
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Horario Definido");
// Mensagem de confirmação da escolha da hora
 Cursor=4;
 Linha=1;
 mostraRELOGIO();
// Mostra o relógio no LCD
 delay(3000);
 z++;
 lcd.clear();
 }
 lcd.setCursor(0,0);
 lcd.print("Hora: ");
 Cursor=6;
 Linha=0;
 relogioFINAL(); // Função para o funcionamento do relógio
 mostraRELOGIO(); // Mostra o relógio digital no LCD
 serve_racao();
// Compara a hora atual com a hora agendada, se for igual, motor ração é ligado
 serve_agua(); 
// Faz a leitura do sensor de nível, se for baixo liga motor da água
 proxHORA(); // Mostra os horários da agenda
 }
}
//Inicializar//
void inicializar() {
 lcd.begin(16, 2);
 //
 lcd.setCursor(0,0); //
 lcd.print("Alimentador para"); // Mensagem de Inicialização do Equipamento
 lcd.setCursor(4,1); //
 lcd.print("Animais"); //
 delay(3000);
 lcd.clear();
}
//Inicializar//
//tempo_racao//
void tempo_racao() {
// Função responsável por receber os horários para serem armazenados na agenda

 lcd.setCursor(0,0);
 lcd.print("Defina o Horario");
 lcd.setCursor(0,1);
 lcd.print("da Racao:");
 delay(3000);
 lcd.clear();
 Cursor=4;
 Linha=1;
 while (x==0) {
 lcd.setCursor(0,0);
 lcd.print("Ajuste 1a Hora:");
 Cursor=4;
 Linha=1;
 limite();
// Limite do relogio (60 seg, 60 min, 24 horas)
 ajusteRELOGIO();
// Função que reconhece qual botão foi pressionado para o ajuste do relógio
 piscar();
// Pisca os dígitos onde o cursor está selecionado
 hora_racao[0]= set[0]; // Define a 1ª hora da agenda
 minuto_racao[0]= set[1]; // Define o 1º Minuto da agenda 
 seg_racao[0]= set[2]; // Define o 1º Segundo da agenda
 }
 x=0;
 thisTime=0;
 delay(1000);
 while (x==0) {
 lcd.setCursor(0,0);
 lcd.print("Ajuste 2a Hora:");
 Cursor=4;
 Linha=1;
 limite(); // Limite do relógio
 ajusteRELOGIO();
// Função que reconhece qual botão foi pressionado para o ajuste do relógio
 piscar();
// Pisca os dígitos onde o cursor está selecionado
 hora_racao[1]= set[0]; // Define a 2ª hora da agenda
 minuto_racao[1]= set[1]; // Define o 2º Minuto da agenda
 seg_racao[1]= set[2]; // Define o 2º Segundo da agenda
 }
 x=0;
 thisTime=0;
 delay(1000);
 if (vezes==3){
 while (x==0) {
 lcd.setCursor(0,0);
 lcd.print("Ajuste 3a Hora:");
 Cursor=4;
 Linha=1;
 limite(); // Limite do relógio
 ajusteRELOGIO();
// Função que reconhece qual botão foi pressionado para o ajuste do relógio
 piscar();
// Pisca os dígitos onde o cursor está selecionado
 hora_racao[2]= set[0]; // Define a 3ª hora da agenda
 minuto_racao[2]= set[1]; // Define o 3º Minuto da agenda
 seg_racao[2]= set[2]; // Define o 3º Segundo da agenda
 }
 x=0;
 thisTime=0;
 delay(1000);
 }
 zera_relogio();
 lcd.clear();
}

//tempo_racao//
//serve_racao//
void serve_racao() {
// Função que compara o horário real com os horários da agenda. Caso iguais, liga
//motor de ração
 if (set[0] == hora_racao[0] && set[1] == minuto_racao[0] && seg_racao[0] == set[2])
{
 for (int p=0 ; p<6; p++){
 motor_racao();}
// Liga motor ração

 }
 if (set[0] == hora_racao[1] && set[1] == minuto_racao[1] && seg_racao[1] == set[2]
){
 for (int p=0 ; p<6; p++){
 motor_racao();}
// Liga motor ração
 }
 if (vezes==3){
 if (set[0] == hora_racao[2] && set[1] == minuto_racao[2] && seg_racao[2] ==
set[2]){
 for (int p=0 ; p<6; p++){
 motor_racao();}
// Liga motor ração
 }}}
//serve racao//
//serve agua//
void serve_agua() {
// Função que faz a leitura do sensor de nível caso o nível esteja baixo, liga motor
//de água.
 if(A==0){

 if (digitalRead (sensor)==LOW){ // Leitura digital do sensor
 A=1;
 seg_agua = set[2];
 if (seg_agua >= 55) {
 seg_agua5 = (5 - (60 - seg_agua));
 }
 if (seg_agua < 50){
 seg_agua5 = (seg_agua + 5);
 } 

 // Compensação caso os segundos estejam acima de 55 segundos, para não ocorrer erro.
 for (int p=0 ; p<2; p++){
 direcao=0;
 motor_agua(); // Liga motor água
 }
 r=1;
 }}
 if (r==1){
 if (set[2] == seg_agua5){
// Após 5 segundos faz motor fechar o dispositivo
 for (int p=0 ; p<2; p++){
 direcao=1;
 motor_agua();
 A=0;
 }}}
}
//serve agua//
//Contador relogio Final//
void relogioFINAL() {
//Função do contador dos segundos do relógio

 static unsigned long ult_tempo = 0;
 int tempo = millis();
 if(tempo - ult_tempo >= 1000) {
 ult_tempo = tempo;
 set[2]++;
 }
 limite();
}
//Contador Relogio Final//
//Limite//
void limite() {
// Função para definição do limite do relógio digital (60seg, 60min, 24hor)
 if(set[2]>=60) { // Limite superior de 60 segundos
 set[2] = 0;
 set[1]++;
 }
 if(set[1]>=60) { // Limite superior de 60 minutos
 set[1] = 0;
 set[0]++;
 } 

 if(set[0]>=24) { // Limite superior de 24 horas
 set[0]=0;
 set[1]=0;
 }
 if(set[2]<0) { // Limite inferior de 0 segundo
 set[2]=59;
 set[1]--;
 }
 if(set[1]<0) { // Limite inferior de 0 minuto
 set[1]=59;
 set[0]--;
 }
 if(set[0]<0) { // Limite inferior de 0 hora
 set[0]=23;
 }
}
//Limite//
//zera_relogio//
void zera_relogio(){
// Zera o relógio para inserir próximo horário na agenda
 set[0]=0;
 set[1]=0;
 set[2]=0;
}
//zera_relogio//
//Função Ajuste RELOGIO//
void ajusteRELOGIO() {
 int botao;
 botao = analogRead (0); // Leitura do pino de entrada analógico
 if (botao < 50) { // botão DIREITA
 thisTime++;
 delay (400);
 }
 else if (botao < 200) { // botão CIMA
 set[thisTime]++;
 if (thisTime==2) {
 delay(130); // Delay curso para segundos
 }
 else delay (250);
// Delay longo para minutos e segundos para aumentar mais lentamente
 }

 else if (botao < 400) { // botão BAIXO
 set[thisTime]--;
 if (thisTime==2) {
 delay(130);
 }
 else delay (250);
 }
 else if (botao < 600) { // botão ESQUERDA
 thisTime--;
 delay (400);
 }
 else if (botao <800) { // botão SELECT
 x++;
 }
 if (thisTime >2 ) { // Limite do cursor
 thisTime = 0;
 }
 if(thisTime <0 ) { // Limite do cursor
 thisTime = 2;
 }
 limite();
}
//Função Ajuste RELOGIO//
//Função Mostra RELOGIO//
void mostraRELOGIO() {
//Função que mostra o relógio digital no formato HH:MM:SS
 if (set[0] < 10) {
 lcd.setCursor(Cursor,Linha);
 lcd.print("0");
 lcd.print (set[0]); // HORA
 }
 else {
 lcd.setCursor(Cursor,Linha);
 lcd.print (set[0]);
 }
 lcd.print (":");
//---
 if (set[1] < 10) {
 lcd.print("0");
 lcd.print (set[1]); // MINUTO
 } 

 else {
 lcd.print (set[1]);
 }
 lcd.print (":");
//---
 if (set[2] < 10) {
 lcd.print("0");
 lcd.print (set[2]); // SEGUNDO
 }
 else {
 lcd.print (set[2]);
 }}
//Função Mostra RELOGIO//
//Piscar//
void piscar() {
// Função para piscar os dígitos selecionados pelo cursor
 static unsigned long ult_temp1 = 0;
 int temp1 = millis();
 static unsigned long ult_temp2 = 0;
 int temp2 = millis();
 if(temp1 - ult_temp1 >= 200) {
 ult_temp1 = temp1;
 if (thisTime==0){
 lcd.setCursor(Cursor, Linha);
 lcd.print(" ");
 }
 else if (thisTime==1){
 lcd.setCursor(Cursor+3, Linha);
 lcd.print(" ");
 }
 else if (thisTime==2){
 lcd.setCursor(Cursor+6, Linha);
 lcd.print(" ");
 }
 if(temp2 - ult_temp2 >= 400) {
 ult_temp2 = temp2;
 mostraRELOGIO();
 }}}
//Piscar//
//MOTOR_RACAO// 

void motor_racao() { // Função para ligar o motor da ração
 int p=0;
 for (int p=0 ; p < 400; p++){
 digitalWrite(pino_direcao_racao, 0);
 digitalWrite(pino_passo_racao, 1);
 delay(1);
 digitalWrite(pino_passo_racao, 0);
 delay(1);}
}
//MOTOR_RACAO//
//MOTOR_AGUA//
void motor_agua() { // Função para ligar o motor de água

 for (int q=0 ; q < 100; q++){
 digitalWrite(pino_direcao_agua, direcao);
 digitalWrite(pino_passo_agua, 1);
 delay(1);
 digitalWrite(pino_passo_agua, 0);
 delay(1);}
 r=0;
}
//MOTOR_AGUA//
//setVEZES//
void setVEZES() { // Função para definir entre 2 ou 3 vezes ao dia
 int botao;
 botao = analogRead (0); // Leitura do pino de entrada analógico
 if (botao < 50) { // botão DIREITA
 vezes++;
 delay(400);
 }
 else if (botao < 200) { // botão CIMA
 vezes++;
 delay (400);
 }
 else if (botao < 400) { // botão BAIXO
 vezes--;
 delay(400);
 } 

 else if (botao < 600) { // botão ESQUERDA
 vezes--;
 delay(400);
 }
 else if (botao <800) { // botão SELECT
 x++;
 }
 if (vezes > 3 ) { // limite do número de vezes
 vezes = 2;
 }
 if(vezes < 2 ) { // limite do número de vezes
 vezes = 3;
 }}
//setVEZES//
//numVEZES//
void numVEZES() { // Função para piscar e confirmar número de vezes ao dia
 lcd.setCursor(0,0);
 lcd.print("2 ou 3 vezes");
 lcd.setCursor(0,1);
 lcd.print("ao dia?"); // Mensagem perguntando se 2 ou 3 vezes ao dia
 while (x==0) {
 static unsigned long ult_temp1 = 0;
 int temp1 = millis();
 static unsigned long ult_temp2 = 0;
 int temp2 = millis();
 setVEZES();
 if(temp1 - ult_temp1 >= 200) {
 ult_temp1 = temp1;
 lcd.setCursor(8 , 1);
 lcd.print(" "); // pisca cursor
 if(temp2 - ult_temp2 >= 400) {
 ult_temp2 = temp2;
 lcd.setCursor(8,1);
 lcd.print(vezes);
 }
 }
 }
 lcd.clear(); 
 lcd.setCursor(0,0);
 lcd.print("Voce escolheu ");
 lcd.print(vezes);
 lcd.setCursor(0,1);
 lcd.print("vezes por dia"); // Mensagem de confirmação da escolha
 delay(3000);
 x=0;
 lcd.clear();
}
//numVEZES//
//proxHORA//
void proxHORA(){ // Função para mostrar horários agendados
 int botao;
 botao = analogRead (0);
 if (botao < 50) { // botão DIREITA
 proxH++;
 delay(250);
 }
 else if (botao < 200) { // botão CIMA
 }
 else if (botao < 400) { // botão BAIXO
 }
 else if (botao < 600) { // botão ESQUERDA
 proxH--;
 if (proxH==0)
 proxH--;
 delay(250);
 }
 else if (botao <800) { // botão SELECT
 }
 if (proxH != 0) {
 if (proxH > vezes ) {
 proxH = 1;
 }
 if(proxH < 1 ) {
 proxH = vezes;
 }
 if (cont != set[2]) {
 cont = set[2];
 y++;
 } 

 if (y>0 && y<=15){ // mostrar por 15 segundos
 lcd.setCursor(0,1);
 lcd.print("Agenda: ");
 if(hora_racao[(proxH-1)]>=10){
 lcd.print(hora_racao[(proxH-1)]);
 }
 else{
 lcd.print("0");
 lcd.print(hora_racao[(proxH-1)]);
 }
 lcd.print(":");
 if(minuto_racao[(proxH-1)]>=10){
 lcd.print(minuto_racao[(proxH-1)]);
 }
 else{
 lcd.print("0");
 lcd.print(minuto_racao[(proxH-1)]);
 }
 lcd.print(":");
 if(seg_racao[(proxH-1)]>=10){
 lcd.print(seg_racao[(proxH-1)]);
 }
 else {
 lcd.print("0");
 lcd.print(seg_racao[(proxH-1)]);
 }}
 if (y>15){
 y=0;
 proxH=0;
 lcd.clear();
 }}}
 //proxHORA-//
