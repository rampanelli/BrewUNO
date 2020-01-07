# BrewUNO

High precision and accuracy, open source, online, extensible and low cost homebrew automation on top of ESP8266 microchip.

![BrewUNO](http://brewuno.com/images/home-brewuno.png)

## Features:
* 100% mobile interface
* Brew with Start/Stop/Resume 
* Responsive interface with stats
* PID control to heat mash SSR
* PID control to heat sparge SSR
* Mash configurations with different steps/temperature and recirculation
* Boil configurations with hops additions
* Configurable WiFi
* Configurable access point
* Synchronization with NTP
* The ability to perform OTA updates
* LCD display

## Getting Started

### Simple Wiring and construction:

![WireUP](http://brewuno.com/images/simplified-setup-BrewUNO.png)

This is the easiest way to mount your brewuno, you can improve it!

### Installing with Flash Download Tools ([video](http://www.brewuno.com/download/install.mp4))

* Download [bin files](https://github.com/rampanelli/BrewUNO/tree/master/bin)
* Download [Flash Download Tools](https://www.espressif.com/sites/default/files/tools/flash_download_tools_v3.6.6_0.zip)
* Setup like this: [setup](http://brewuno.com/download/esp8266-download-tool.png)
* firmware.bin offset: 0x0
* spiffs.bin offset: 0x00300000
* Click ERASE button
* Click START button
* Connect to BrewUNO wifi (SSID: BrewUNO, Password: brew-uno). AP mode ip: 192.168.4.1
* Scan and configure your wifi under menu

### Installing & Deployment with VS Code

### Prerequisites

You will need the following before you can get started.

* [VS Code](https://code.visualstudio.com/)
* [PlatformIO](https://platformio.org/) - IDE for development **with ESP8266 v2.2.2 platform installed**
* [NPM](https://www.npmjs.com/) - For building the interface (if you want)
* Bash shell, or Git Bash if you are under windows

Pull the project and add it to PlatformIO as a project folder (File > Add Project Folder).

PlatformIO should download the ESP8266 platform and the project library dependencies automatically.

Once the platform and libraries are downloaded the back end should be compiling.

Edit /data/config/wifiSettings.json with yours wifi configurations.

Under PlatformIO menu: Upload File System image, and then, Upload and Monitor. You will see the ip address to access interface. AP mode ip: 192.168.4.1

### Building the interface

The interface has been configured with create-react-app and react-app-rewired so the build can customized for the target device. The large artefacts are gzipped and source maps and service worker are excluded from the production build.

You will find the interface code in the ./interface directory. Change to this directory with your bash shell (or Git Bash) and use the standard commands you would with any react app built with create-react-app:

#### Download and install the node modules

```bash
npm install
```

#### Build the interface

```bash
npm run build
```

**NB: The build command will also delete the previously built interface (the ./data/www directory) and replace it with the freshly built one, ready for upload to the device.**

#### Running the interface locally

```bash
npm start
```

**NB: To run the interface locally you will need to modify the endpoint root path and enable CORS.**

The endpoint root path can be found in .env.development, defined as the environment variable 'REACT_APP_ENDPOINT_ROOT'. This needs to be the root URL of the device running the back end, for example:

```
REACT_APP_ENDPOINT_ROOT=http://192.168.0.6/rest/
```

CORS can be enabled on the back end by uncommenting the -D ENABLE_CORS build flag in platformio.ini and re-deploying.

## Configuration

Standard configuration settings, such as build flags, libraries and device configuration can be found in platformio.ini. See the [PlatformIO docs](http://docs.platformio.org/en/latest/projectconf.html) for full details on what you can do with this.

By default, the target device is "esp12e". This is a common ESP8266 variant with 4mb of flash though any device with at least 2mb of flash should be fine. The settings configure the interface to upload via serial by default, you can change the upload mechanism to OTA by uncommenting the relevant lines.

As well as containing the interface, the SPIFFS image (in the ./data folder) contains a JSON settings file for each of the configurable features. The config files can be found in the ./data/config directory:

File | Description
---- | -----------
apSettings.json | Access point settings
ntpSettings.json | NTP synchronization settings
otaSettings.json | OTA Update configuration
wifiSettings.json | WiFi connection settings

The default settings configure the device to bring up an access point on start up which can be used to configure the device:

* SSID: BrewUNO
* Password: brew-uno

# Thanks to:

* Luciano Rampanelli
* Eduardo 'Dumpa' Sanches
* Guilherme Wood
* Paiakan
* [ESP8266 React](https://github.com/rjwats/esp8266-react) - rjwats/esp8266-react

## Alexa Integration
Olá cervejeiros,

Esta é uma simulação passo a passo do BrewUNO sofware para controle de brassagem que esta prestes a receber a integração com o assistente pessoal da amazon.

O BrewUNO é desenvolvido pelo Bruno Leitão e conta com apoio de diversas pessoas dentre elas esse que vos fala, caso deseje dar o seu apoio, acesse brewuno.com e faça uma doação ou acesse o repositório github e contribua com sugestões e opiniões.

Antes de iniciar, é importante que você conheça os comandos do BrewUNO pelo teclado, são eles:

Botão 1 - Clique longo - Inicia/Termina o processo de brassagem ou mostura.
	- Clique curto - Pausa/Retorna o processo (mostura/fervura) em andamento.
Botão 2 - Clique longo - Inicia/Termina o processo de fervura.
	- Clique curto - Decrementa a potência PWM onde permitido.
Botão 3 - Clique longo - Avança para o próximo passo.
	- Clique curto - Incrementa a potência PWM onde permitido.
Botão 4 - Clique curto - Liga ou desliga a bomba/circulação.
	- Clique longo - Não definido.


Além disso é importante ficar de olho no display onde se pode identificar cada fase do processo:

 - No canto superior esquerdo temos o símbolo do sinal wifi indicando o tipo de conexão, AP ou STATION.
 - Ainda na primeira linha a identificação BrewUNO seguida da versão e release, que será substituída pelo timer sempre que um processo estiver em andamento.
 - Na segunda linha temos a temperatura do sensor H1 seguida do setpoint de temperatura para o processo em andamento. Mais a frente temos o % PWM para H1.
 - Ainda na segunda linha ao final temos a letra P (Pausa da bomba) ou o ícone de uma gota, indicando que o relé da bomba está ligado.
 - Na terceira linha temos a temperatura do sensor H2 seguida do setpoint de temperatura para o processo em andamento. Mais a frente temos o % PWM para H2.
 - Na quarta linha quando ligamos o equipamento pela primeira vez aparece o IP do BrewUNO em funcionamento no modo AP: 192.168.4.1, para conectar-se busque a rede BrewUNO e utilize a senha brew-uno. Após conectar-se você deve acessar o IP pelo browser e fazer as configurações para conexão na sua rede local para que o BrewUNO tenha acesso a internet e seja possível sincronizar o timer/ntp. Faça isso pelo endereço http://192.168.4.1/wifi-configuration, clique em network scanner, aguarde o escaneamento, clique sobre a rede wifi escolhida, preencha o campo Password com a senha dessa rede e clique em SAVE/SALVAR. Após executar este procedimento pode ser necessário reiniciar o BrewUNO e reconectar agora pelo novo IP que irá aparecer no display após a conexão pela rede local.
 - Durante o processo de brassagem, poderão aparecer logo abaixo do ícone da gota/P no final da linha 3, as letras M que significa que o processo em execução é a mostura, a letra B indicando que o processo em execução é a fervura, ou ainda a letra L indicando que o BrewUNO encontra-se no estado de bloqueio/Locked aguardando que seja executada alguma tarefa externa, para sair desse estado deve-se segurar o botão 3 pressionado até ouvir um bipe. 
 	
Agora que você já sabe utilizar o teclado, identificar as informações importantes no display e configurar o wifi, vamos em frente.

O assistente Alexa irá suportar diversos comandos nativos sem necessidade de configurações adicionais pelo BrewUNO ou criação de rotinas personalizadas pelo app Amazon Alexa. E lembre, antes você precisa estar com o seu dispositivo (Echo Dot, Echo View ou outro) ligado a sua rede wifi e executar o comando "Alexa, buscar dispositivos" ou pelo aplicativo/site o comando equivalente.

Alexa, ativar/ligar desativar/desligar mostura. (inicia o processo da brassagem que inclui a fervura ao final)
Alexa, ativar/ligar desativar/desligar fervura. (inicia o processo de fervura avulsa)
Alexa, ativar/ligar avanço. 
Alexa, ativar/ligar circulação. (liga a bomba)
Alexa, desativar/desligar bomba.
Alexa, ativar/ligar pausa. (pausa o processo, desligando todas as resistências e a bomba)
Alexa, ativar/ligar resumo. (retorna da pausa para o estado anterior)
Alexa, ajustar o PWM em 55 por cento. (ajusta o PWM durante o processo onde for permitido esse ajuste também pelos botões 2 e 3)
Alexa, desligue o brewuno. (desliga todos os processos em andamento)

Você também pode criar rotinas personalizadas através do app Amazon Alexa do seu celular ou acessando sua conta da amazon pelo site https://alexa.amazon.com.br. 

Exemplos de rotinas:
Alexa, iniciar brassagem. (comando nativo: ligar mostura)
Alexa, ligar a bomba (comando nativo: ativar circulação)
Alexa, ajustar máxima potência. (ajustar o PWM em 100 por cento)
Alexa, ajustar meia potência. (ajustar o PWM em 50 por cento)
Alexa, ajustar 1 grau por minuto. (ajustar o PWM em XX por cento, referente a potência necessária para seu equipamento/receita para a subída de 1°C por minuto)

Ou ainda durante a execução da fervura criar timers personalizados:
Alexa, criar timer de 50 minutos para lúpulo de aroma. (cria um timer que avisará dentro de 50 minutos o momento para adicionar o lúpulo)




