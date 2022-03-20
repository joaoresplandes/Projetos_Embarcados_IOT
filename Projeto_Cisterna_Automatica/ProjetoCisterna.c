#include<18f4550.h>
#fuses HS, NOWDT, NOLVP
#use delay(clock=20000000)

#define Nivel_Boia_Alto				pin_b1 // Boia de Nivel Alto da Cisterna
#define Nivel_Boia_Baixo			pin_b2 // Boia de Nivel Baixo da Cisterna
#define Nivel_Baixo_Comporta		pin_c5 // Boia de Nivel Baixo da Comporta
#define Chave_Manual_Automatico		pin_b0 // CHAVE SELETORA DE MODO AUTOMATICO OU MANUAL
#define LED_Automatico				pin_b3 // LED_BLUE - Indica Sistema Automatico Funcionando
#define LED_Manual					pin_b4 // LED_GREEN -  Indica Sistema Manual Funcionando
#define BUZZER						pin_b5 // Buzzer
#define LED_Irrigacao_Desligada		pin_b6 // LED que indica bomba da cisterna não funcionado
#define LED_Bomba_Ligada			pin_b7 // LED que indica que a bomba da cisterna está ligada
#define LED_Bomba_Desligada			pin_c0 // LED que indica que a bomba da cisterna está desligada
#define Bomba_Cisterna				pin_c1 // Liga / Desliga a Bomba da Cisterna
#define Comporta_Bomba_2			pin_c2 // Bomba para encher o reservatório principal
#define LED_Comporta				pin_c4 // LED da Comporta (Bomba Comporta)

///LEDs Boias///

#define LED_Nivel_Boia_Alto			pin_a0 // LED da Boia de Nivel Alto da Cisterna
#define LED_Nivel_Boia_Baixo		pin_a1 // LED da Boia de Nivel Baixo da Cisterna
#define LED_Nivel_Baixo_Comporta	pin_a2 // LED da Boia de Nivel Baixo da Comporta

/// BOTÕES MANUAIS ///

#define botao_liga					pin_a3 // Botão que Liga a Bomba da Cisterna no Modo Manual
#define botao_desliga				pin_a4 // Botão que Desliga a Bomba da Cisterna no Modo Automatico

/*
Tempo de execução do processo automatico se da no periodo de "1h10",
no qual estes 10min se dão pelo tempo de irrigacao:
Ou seja - > 1h tempo de comporta aberta ou fechada caso o nivel alto da
cisterna esteja indique que a mesma encontra-se em sua totalidade.
e os 10 min para a irrigacao, podendo interromper quando o NIVEL BAIXO,
indique que a cisterna está quase vazia, mesmo assim, o tempo de irrigacao
continua a contar. 
*/

int i; // tempo de irrigação
int c; // conta quantas vezes o BUZZER vai tocar no modo automatico antes da irrigação começar
char cod_display[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
int tempo; // tempo para encher a cisterna principal


/// FUNÇÕES DE LIGA E DESLIGA BOMBA DA CISTERNA ///
	/// ESTRUTURA DE LIGAMENTO DA BOMBA DA CISTERNA
void liga_bomba_cisterna(){
	output_high(LED_Bomba_Ligada); // Liga LED de bomba da cisterna ligada
	output_high(Bomba_Cisterna); // Liga bomba da cisterna
	output_low(LED_Bomba_Desligada); // Desliga o LED que indica que a bomba da cisterna está desligada
}

	/// ESTRUTURA DE DESLIGAMENTO DA BOMBA DA CISTERNA 
void desliga_bomba_cisterna(){
	output_low(LED_Bomba_Ligada); // Desliga o LED que indica que a bomba da cisterna está ligada
	output_low(Bomba_Cisterna); // Desliga a bomba da cisterna
	output_high(LED_Bomba_Desligada); /// Liga o LED que indica que a bomba da cisterna está desligada
}

/// FUNÇÕES DE LIGA E DESLIGA BOMBA DA COMPORTA///
	/// ESTRUTURA DE LIGAMENTO DA BOMBA DA COMPORTA
void liga_bomba_comporta(){
	output_high(LED_Comporta); // Liga do LED para abertura da bomba
	output_high(Comporta_Bomba_2); // Abre a comporta (Liga a bomba Comporta)
}

	/// ESTRUTURA DE DESLIGAMENTO DA BOMBA DA COMPORTA
void desliga_bomba_comporta(){
	output_low(LED_Comporta); // Desliga do LED para abertura da bomba
	output_low(Comporta_Bomba_2); // Abre a comporta (Desliga a bomba Comporta)
}

///LEDs BOIAS///
void leds_boias(){
	if(!input(Nivel_Boia_Alto)){
		output_high(LED_Nivel_Boia_Alto);
	}
	else{
		output_low(LED_Nivel_Boia_Alto);
	}
	if(!input(Nivel_Boia_Baixo)){
		output_high(LED_Nivel_Boia_Baixo);
	}
	else{
		output_low(LED_Nivel_Boia_Baixo);
	}
	if(!input(Nivel_Baixo_Comporta)){
		output_high(LED_Nivel_Baixo_Comporta);
	}
	else{
		output_low(LED_Nivel_Baixo_Comporta);
	}
}

/// lIGA O BUZZER 2 VEZES ANTES DA IRRIGAÇÃO /// 
void liga_buzzer(){
	for(c = 0; c<2; c++){
		output_high(BUZZER); // Liga o buzzer
		delay_ms(500);
		output_low(BUZZER); // Desliga o buzzer
		delay_ms(500);
	}
}

/// VERIFICAÇÃO DA COMPORTA E NIVEL ALTO ///
void verifica_alto(){
	if(input(Nivel_Baixo_Comporta)){ // Verifica se a boia de NIVEL BAIXO DA COMPORTA está no nivel para o funcionamento da bomba
		if(input(Nivel_Boia_Alto)){ // Verifica se a boia de NIVEL ALTO DA CISTERNA está indicando ou não que a mesma está cheia
			liga_bomba_comporta();
			desliga_bomba_cisterna();
		}
		else{
			leds_boias();
			desliga_bomba_comporta();
			output_low(LED_Irrigacao_Desligada); // Desliga LED que indica irrigação está desligada
		}
	}
	else{
		leds_boias();
		desliga_bomba_comporta();
		output_low(LED_Irrigacao_Desligada); // Desliga LED que indica enchendo a cisterna
	}
}

/// IRRIGAÇÃO COM DURAÇÃO DE 10 MINUTOS ///
void irrigacao(){
	desliga_bomba_comporta();
	for(i = 1; i<=10; i++){
		liga_bomba_cisterna();
		output_d(cod_display[10-i]); // Mostra no display de 7 segmentos a contagem de 9 a 0
		delay_ms(500);
		leds_boias(); // Verifica o nivel das boias e caso alguma aprensente nivel lógico 1, à acende
		if(input(Chave_Manual_Automatico)){ // Se a chave estiver em nivel lógico baixo (Manual) finaliza a sessão
			break;
		}
		if(!input(Nivel_Boia_Baixo)){ // Verifica se a boia de NIVEL BAIXO DA CISTERNA está abaixo do nivel permitido para o funcionamento
			desliga_bomba_cisterna();
			output_d(cod_display[0]); // Liga o display de 7 segmentos com o numero 0
			i = 0;
			break;
		}	
	}
}

/// ENCHENDO A CISTERNA ///
void tempo_irrigacao(){
	for(tempo = 0; tempo <=20; tempo++){ // Realiza a contagem do tempo antes da irrigação
		if(input(Chave_Manual_Automatico)){ // Se a chave estiver em nivel lógico baixo (Manual) finaliza a sessão
			break;
		}
		verifica_alto(); // Verifica as boias de nivel alto da cisterna e nivel baixo da comporta.
		leds_boias(); // Verifica as boias do sistema e se alguma aprensentar nivel lógico 1, à acende
		desliga_bomba_cisterna();

		// Liga o LED indicando que a Irrigação da bomba da cisterna está initerrompida
		output_high(LED_Irrigacao_Desligada); 
		delay_ms(500);
		output_low(LED_Irrigacao_Desligada);
		delay_ms(500);
	}
}

/// VERIFICAÇÃO DO NIVEL BAIXO DA BOMDA DA CISTERNA PARA A LIGAÇÃO DA MESMA ///
void verif_bomba_cisterna(){
	if(!input(Nivel_Boia_Baixo)){ // Verifica se o NIVEL BAIXO CISTERNA não está no nivel para o funcionamento da bomda NIVEL 0
		desliga_bomba_cisterna();
		leds_boias(); 
	}
	else{ // CASO a condição anterior não seja verdadeira:
		liga_buzzer(); // Liga o Buzzer
		irrigacao(); // chama a função "irrigacao()" para ligar a bomba e começar a irrigação durante "10 minutos"
	}
}

/// VERIFICAÇÃO DO NIVEL BAIXO MANUAL///
void liga_manual_bomba(){
	if(!input(botao_liga) && input(Nivel_Boia_Baixo)){ // Verifica se o BOTAO LIGA foi prossionado e se a BOIA DE NIVEL BAIXO está com nivel alto
		delay_ms(300);
		while(input(Chave_Manual_Automatico)){ /* Cria um loop no qual a bomba da cistena e todos os seus componentes funcionam junto com a 
			verificação do nivel da boia inferior e o botão de desligar bomba da cisterna - Enquanto a Chave Manual Automatico estiver aberta */
			liga_bomba_cisterna();
			desliga_bomba_comporta();
			output_low(LED_Irrigacao_Desligada); // Desliga o LED Irrigação Desligada
			if(!input(Nivel_Boia_Baixo) || !input(botao_desliga)){ // Verifica se a boia inferior ficou a baixo do nivel ou se o botao desliga foi pressionado
				break;
			}	
		}
	}
	else{
		verifica_alto(); // Verifica as boias de nivel alto da cisterna e nivel baixo da comporta
		leds_boias(); // Verifica a situação das boias e acende o LED caso alguma das boias apresente um nivel lógico 1
		output_high(LED_Irrigacao_Desligada); // Liga LED de Irrigação Desligada
	}
}

/// CHAMADA PRINCIPAL ///
void main(){
	while(TRUE){
		///////Inicio//////
		while(!input(Chave_Manual_Automatico)){ // Sistema Automatico
			delay_ms(300);
			output_high(LED_Automatico); // Liga LED do modo Automatico
			output_low(LED_Manual); // Desliga o LED do modo Manual
			tempo_irrigacao(); // Chama a função que começa a encher a cisterna
			if(input(Chave_Manual_Automatico)){ // Se a chave estiver em nivel lógico baixo (Manual) finaliza a sessão
				break;
			}
			verif_bomba_cisterna(); // Depois chama a a função que verifica o NIVEL BAIXO da cisterna e libera bomba de irrigação.
			if(input(Chave_Manual_Automatico)){ // Se a chave estiver em nivel lógico baixo (Manual) finaliza a sessão
				break;
			}
		}
		while(input(Chave_Manual_Automatico)){ // Sistema Manual
			delay_ms(300);
			output_d(cod_display[0]); // Modo manual, o display irá mostrar 0
			output_high(LED_Manual); // Liga LED do modo manual
			output_low(LED_Automatico); // Desliga LED do modo Automatico
			liga_manual_bomba(); // Chama a função de ligar a bomba no modo manual
			
			if(input(!Chave_Manual_Automatico)){ // Se a chave estiver em nivel lógico alto (Automatico) finaliza a sessão
				break;
			}
		}
	}
}

				