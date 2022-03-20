# Projetos
## Neste respositório constitui-se os seguintes projetos
### Projetos_Embarcados
- Cisterna_Automatica

## Cisterna Automática

#### Contexto do Projeto
- Desenvolvido para controlar de forma automática a irrigação de uma cisterna, mas tambem sendo possível o controle da mesma por meio de um modo manual, assim reduzindo desperdicio e custos.

#### Desenvolvimento de Problema
- O objetivo é controlar a captação da água da chuva por meio da cisterna de captação e enviar para a cisterna principal, a qual será responsavel pela irrigação ou distribuição da água para determinados locais que o projeto for implementado.

#### Componetes
##### Principais
- Nivel_Boia_Alto		        -> Boia de Nivel Alto da Cisterna
- Nivel_Boia_Baixo			    -> Boia de Nivel Baixo da Cisterna
- Nivel_Baixo_Comporta		  -> Boia de Nivel Baixo da Comporta
- Chave_Manual_Automatico		-> CHAVE SELETORA DE MODO AUTOMATICO OU MANUAL
- LED_Automatico				    -> LED_BLUE - Indica Sistema Automatico Funcionando
- LED_Manual					      -> LED_GREEN -  Indica Sistema Manual Funcionando
- BUZZER						        -> Buzzer
- LED_Irrigacao_Desligada	  -> LED que indica bomba da cisterna não funcionado
- LED_Bomba_Ligada		      -> LED que indica que a bomba da cisterna está ligada
- LED_Bomba_Desligada			  -> LED que indica que a bomba da cisterna está desligada
- Bomba_Cisterna				    -> Liga / Desliga a Bomba da Cisterna
- Comporta_Bomba_2			    -> Bomba para encher o reservatório principal
- LED_Comporta				      -> LED da Comporta (Bomba Comporta)

##### LEDs Boias

- LED_Nivel_Boia_Alto			  -> LED da Boia de Nivel Alto da Cisterna
- LED_Nivel_Boia_Baixo	    -> LED da Boia de Nivel Baixo da Cisterna
- LED_Nivel_Baixo_Comporta	-> LED da Boia de Nivel Baixo da Comporta

##### Botões Manuais 

- botao_liga					      -> Botão que Liga a Bomba da Cisterna no Modo Manual
- botao_desliga				      -> Botão que Desliga a Bomba da Cisterna no Modo Automatico

#### Controle Automático
- O controle automático, que deve ser indicado por LED_Automatico, que num primeiro momento verifica o nivel alto da cisterna principal e o nivel mínimo da cisterna de captação para iniciar o preenchimento da cisterna principal durante 1h, durante este período, o LED_Irrigação_Desliaga indica que o preenchimento da cisterna está ocorrendo e LED_Bomba_Desliaga indicando que a bomba da mesma está desligado e a Bomba da Cisterna é ligada. Após este período, um BUZZER será ligado, indicando que a irrigação da cisterna principal será iniciada, logo, LED_Irrigação_Desligada e LED_Bomba_Desligada são desligados para que, durante 10 mínutos, A Bomba_Cisterna e LED_Bomba_Ligada sejam acionados, junto de uma contagem regressiva de 9 a 0 pelo display. E o processo se Repete.
  
#### Controle Manual
- No controle manual, indicado por LED_Manual, um Botao_Liga a irrigação e outro Botao_Desliga. O buzzer deve apitar para avisar que a irrigação iniciará. Durante a irrigação, o display deve piscar um zero (0). 
- Deve-se manter o controle do nível do reservatório descrito no controle automático. 