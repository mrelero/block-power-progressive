# block-power-progressive

 ## Introdução 
 Este projeto tem como objetivo programar um microcontrolador PIC18F2320 para simular o funcionamento de um bloqueador progressivo com relé. O bloqueador funciona da seguinte forma:

 - Normalmente a porta RA0 está conectada em baixo, simulando um sinal de desbloqueio ou a conexão de um determinado dispositivo; 
 - Ao realizar a abertura do circuito em RA0, ou seja, identificação de nível alto em RA0, o dispositivo entra em processo de bloqueio progressivo. 
 - O processo de bloqueio progressivo funciona com chaveamento do relés em tempos que são aumentados gradativamente. 


 ## Hardware

 O Hardware é simples e é composto de um PIC18F2320, um transitor BC337, um relé de 12V e um diodo IN4007 de roda livre. 

 ![](figuras/esquema.png)


## Timers

Foi configurado um timer com interrupção de 10ms para contagem e chaveamento do bloqueio. Para isso, os registradores do PIC foram configurados conforme abaixo: 

### Timer 0

T0CON = 0b10010111;

- Timer Habilitado;
- Prescaler 256;
- Transição low-high;
- Registrador de 16bits; 

### Interrupções

- GIE = 1 > Habilita Interrupções
- PEIE = 1 > Habilita Interrupções
- TMR0IE = 1 > Habilita Interrupção Timer
- TMR0IF = 1 > Seta Flag de Interrupção Timer
- ADIE = 0 > Desabilita Interrupção AD
- RCIE = 0 > Desabilita Interrupção porta C
- TMR0 = 0xFFB1 > Configura os registradores para contagem de tempo igual a 10ms.
- TMR0IP = 1 > Seta o Timer com prioridade alta.
- IPEN = 1 > Seta prioridade de interrupções.


## Conversor AD

A detecção de sinal aberto ou fechado em RA0 é realizada por meio de conversão A/D. Esta abordagem permite trabalhar com margens de segurança de tensão, e assim evitar bloqueios e desbloqueios indevidos. 

O pino RA0 foi configurado conforme os parâmetros:
- TAD = 20us;
- Aquisition Time = 40us;
- Vref = Vdd = 5V;

Portanto, quando a tensão lida em R0 for maior que 0.5V, o processo de bloqueio é iniciado. 

## Maquina de Estados

Em Andamento....

