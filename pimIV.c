/************************************************
* ESTACIONAMENTO PIM LTDA						*
* SISTEMA:      ESTACIONAMENTO					*
* PROGRAMA:     GERENCIADOR DE ESTACIONAMENTO	*
* PROGRAMADORES: Fabio Tang;                    *
* Dyganar Campos de Jesus;                      *
* Claudemir Correa Pinto.                       *
* DATA INICIO: 08/11/2014					    *
* DATA VERS�O: 15/11/2014     					*
* OBSERVA��O:                   				*
* Para acessar consultas digite 3 no menu       *
* Para acessar relat�rio digite 100 no menu     *
* Para acessar manuten��o digite 101 no menu    *
* Para fechar o CAIXA digite 9 no menu          *
************************************************/

#include <stdio.h>      //biblioteca para entradas e sa�das, arquivos, etc.
#include <string.h>     //manipula��o de strings
#include <ctype.h>     //biblioteca para deixar placa em ma�usculo
#include <time.h>      //manipula��o de tempo


//VARIAVEIS GLOBAIS -------------------------------
typedef struct{
	double horainicial;
	double horaadicional;
	double saldo;
	int travaSistema, movimentacaodia, saidasdia, totalvagas, vagainicial, totalVagasLivres, vagasAnterior, totalVagasAnterior;
}backup;
backup dados;
time_t dhEntrada;	// para calculo
time_t dhSaida;		// para calculo
char *placas[1000][5];  
int calculoHoras[1000][2];
FILE *historicoE;
FILE *historicoS;
FILE *historicoG;
//PROT�TIPA��O DE PROCEDIMENTOS ------------------------------
void home();
void entrada();
void saida();
void consulta();
void manutencao();
void lerRelatorio();
void fecharCaixa();

//FUN��O QUE TESTA PLACA XXX0000 -----------------------------
int testplaca(char pplaca[]){
int contadorI, TAM_PLACA, TESTE;
TAM_PLACA=strlen(pplaca);
for(contadorI=0; contadorI<dados.vagainicial; contadorI++){  
	if(strcmp(pplaca, placas[contadorI][1])==0){      //verifica se ve�culo j� foi cadastrado no sistema.
		system("cls");
		printf("Veiculo ja cadastrado, informe outra placa\n");
		entrada();
	}
	
}
	if((TAM_PLACA !=7)||(TAM_PLACA<7)||(TAM_PLACA>7)){
			system("cls");
			printf("Placa invalida");
			entrada();
	}
		for(contadorI=0; contadorI<3; contadorI++){
		TESTE=isalpha(pplaca[contadorI]);//retorna 1 se for letra
			if(TESTE==0){
				system("cls");
				printf("Placa invalida");
				entrada();
			}
		}
			for(contadorI=3; contadorI<7; contadorI++){
			TESTE=isalpha(pplaca[contadorI]);//retorna 1 se for letra
				if(TESTE!=0){
					system("cls");
					printf("Placa invalida");
					entrada();
				}
			}
return(1);
}

// FUN��O QUE RETORNA DATA -----------------------------------
char *tempo_1(){
	char data[20];
	time_t currentTime;
	struct tm *timeInfo;             // Pega a hora atual do sistema e converte-a em uma estrutura tm.
	currentTime= time(NULL);         // captura o tempo corrente
	timeInfo=localtime(&currentTime);// converte para formato local
	strftime(data, 20, "%d.%m.%Y", timeInfo);//salva em formato string 00.00.0000

return(data);                        // retorna ponteiro com string formatada.
}

// FUN��O QUE RETORNA TEMPO PARA FORMATAR HORA
char *tempo_2(){
	char hora[20];
	time_t currentTime;
	struct tm *timeInfo; 
	currentTime= time(NULL);
	timeInfo=localtime(&currentTime);
	strftime(hora, 20, "%H:%M:%S", timeInfo);
return(hora);
}
void backupGeral(){					//fun��o que armazena struct backup/dados em arquivo
historicoG = fopen("historicoGeral.bin","wb");  //cria arquivo historicoGeral.bin
	if(historicoG != NULL){                     //se foi poss�vel criar arquivo
		fwrite(&dados,sizeof(backup),1,historicoG); //faz backup da struct dados
	}
fclose(historicoG);	                           //fecha o arquivo
}
// FUN��O MENU
void home(){
backupGeral();                                //faz backup dos dados correntes
system("cls");                                //limpa tela
int contadorI, condicionalS, switchItem;      //vari�veis locais
double horaAdicionalM;
horaAdicionalM=dados.horaadicional*3600;      //coloca em reais valor de hora adicional.
	for(contadorI=0;contadorI<32;contadorI++){
			printf("*");
		}//----------MENU PRINCIPAL ---------------------------------------
	printf("                               \n");
	printf("         ESTACIONAMENTO        \n");
if(dados.travaSistema==1){
	printf("FECHANDO CAIXA...\nIMPOSSIVEL INSERIR NOVA ENTRADA\n");}
	printf("                               \n");
	printf("     R$%.2f Primeira hora      \n",dados.horainicial);
	printf("     R$%.2f Por hora Adicional \n",horaAdicionalM);
	printf("                               \n");
	printf("   1 -  ENTRADA DE VEICULO     \n");
	printf("   2 -  SAIDA DE VEICULO       \n");
	printf("   3 -  CONSULTA               \n");
	printf("   9 -  FECHAR CAIXA           \n");
	printf("  100 - RELATORIO              \n");
	printf("  101 - MANUTENCAO             \n");
	printf("                               \n");
	printf("LIMITE DE VAGAS: %d            \n",dados.totalvagas);
	printf("VAGAS DISPONIVEIS: %d          \n",dados.totalVagasLivres);  
	printf("VAGAS OCUPADAS: %d             \n",dados.totalvagas-dados.totalVagasLivres); 
		for(contadorI=0;contadorI<32;contadorI++){
			printf("*");
			}
	printf("\n");
	printf("INFORME A OPCAO: ");
    scanf("%d",&switchItem);fflush(stdin);
    switch (switchItem){
    case 1:
        entrada();    //direciona para procedimento entrada
        break;
    case 2:
        saida();     //direciona para procedimento saida
        break;
    case 3:
        consulta(); //direciona para procedimento consulta
        break;
    case 9:
	puts("Tem certeza que deseja sair? s ou n");
	condicionalS=getch();fflush(stdin);
	if((condicionalS=='s')|| (condicionalS=='S')){
		fecharCaixa(); //direciona para procedimento fecharCaixa
		exit(0);
	}
	else
		home();     //retorna para o menu
		break;
    case 100:
    	lerRelatorio();  //direciona para procedimento de leitura de relat�rio
    	break;
    case 101:
    	manutencao();   //direciona para procedimento de manuten��o
        break;
    default:
        printf("\n\nOpcao nao encontrada!\n\n");  //em caso de digitar op��o invalida, apresenta essa informa��o...
        printf("Aperte qualquer tecla para continuar\n");getch();
        system("cls");
        home();                                   //...retornando para o menu principal
    }
}

 //FUN��O ENTRADA VE�CULO
void entrada(){
if(dados.travaSistema==0){               //se n�o estiver em procedimento de fechamento de caixa, continua.
if (dados.totalVagasLivres!=0){          //se h� vagas livres, continua.
			int switchItem, resultadotestplaca, contadorI; //vari�veis locais
			char placa[8];
			char hora[20];
			char data[20];
			char pplaca[8];
			char placaT[8]="XXX0000";
			printf("   ESTACIONAMENTO - ENTRADA\n"); // menu entrada
	    		printf("   PLACA DO VEICULO : ");
	    		scanf("%s",&placa);
			strupr(placa);//deixa caracteres da placa em mai�sculo
			printf(" 1 - CONFIRMA     9 - VOLTAR\n");
			printf("      INFORME A OPCAO: ");
			scanf("%d",&switchItem);
			switch (switchItem){
			    case 1:
				sprintf(pplaca,"%s",placa);
				//testes se a placa est� no padr�o utilizando a fun��o testplaca();
				resultadotestplaca=testplaca(&pplaca);
				if(resultadotestplaca==1){
					placas[dados.vagainicial][1]=pplaca; //armazena placa
					sprintf(data,"%s",tempo_1());        //armazena retorno da fun��o tempo_1 na vari�vel data
					placas[dados.vagainicial][2] = data; //armazena data
					time(&dhEntrada);			         //tempo para calculo
					calculoHoras[dados.vagainicial][1]=dhEntrada; //armazena tempo para calculo
					sprintf(hora,"%s",tempo_2());        //armazena retorno da fun��o tempo_2 na vari�vel hora
					placas[dados.vagainicial][3] = hora; //armazena hora
					dados.vagainicial++;	             //incrementa registro/vaga
				}
				dados.totalVagasLivres = dados.totalVagasLivres - 1; //dando baixa na contagem de vagas livres apos entrada
				dados.movimentacaodia = dados.movimentacaodia + 1; // contando movimenta��o
				//demonstrando/imprimindo em tela  recibo do cliente
					system("cls");
				printf("Dados de Entrada:\n\n");
			  	printf("\nPlaca: %s", pplaca);
			  	printf("\n\n-------------------------------------\n");
			  	printf("\nData de Entrada: %s",data);
			  	printf("\nHorario de Entrada: %s",hora);
			  	printf("\n\n");
				printf("-------------------------------------\n");
				printf("Aperte qualquer tecla para continuar\n");getch();
				//fim da impressao do recibo do cliente
				
				historicoE = fopen("historicoEntrada.txt","a"); //abertura do arquivo para backup de dados de entrada
					fprintf(historicoE,"%s", pplaca);            //dados a serem salvos
					fprintf(historicoE,"%s", data);
					fprintf(historicoE,"%s",hora);
					fprintf(historicoE,"%d",dhEntrada);
					fclose(historicoE);
				historicoS = fopen("historicoSaida.txt","a"); //cria arquivo de saida para compara��o em caso de...
					fprintf(historicoS,"%s", placaT);
					fclose(historicoS);                       //...restaura��o de dados ao iniciar a fun��o main
				home();
				break;
					
				
			    case 9:
			        system("cls");
			        home();
			        break;
			    default:
			        system("cls");
			        printf("\nOpcao nao encontrada!\n");
			        printf("Aperte qualquer tecla para continuar\n");getch();
			        system("cls");
			        home();	
			    }			    
}
else 
	system("cls");	  //informa que estacionamento est� lotado
	puts("\nDesculpe! O estacionamento esta LOTADO. Libere uma vaga primeiro!\n");
	printf("Aperte qualquer tecla para continuar\n");getch();
	home();
}
else
	system("cls");    //Informa que est� em processo de fechamento de caixa
	puts("\nO Caixa esta sendo fechado. Impossivel adicionar veiculos.");
	printf("Aperte qualquer tecla para continuar\n");getch();
	home();
}
// FUN��O SA�DA VE�CULO
void saida(){
int contadorI,contadorJ, switchItem;
char hora[20];
char data[20];
char placa[8]; 
char pplaca[8];
double valor;
int resultado, horaS;
int diferencaIO;
	printf("  ESTACIONAMENTO - SAIDA\n");
    	printf("  PLACA DO VEICULO : ");
    	scanf("%s",&placa);
    	strupr(placa);
    	sprintf(pplaca,"%s",placa);
    	printf("1 - CONFIRMA     9 - VOLTAR\n");
    	printf("      INFORME A OPCAO: ");
    	scanf("%d",&switchItem);
    	switch (switchItem){
    	case 1:	
    		for(contadorI=0;contadorI<dados.totalvagas;contadorI++){					
				if (strcmp(pplaca,placas[contadorI][1])==0){  //funcao q compara 2 strings
					dados.saidasdia=dados.saidasdia+1;
					dados.vagainicial = contadorI; //proxima posicao livre sera a posicao que acabou de ser liberada	
					sprintf(data,"%s",tempo_1());
					placas[dados.vagainicial][4] = data;
					sprintf(hora,"%s",tempo_2());			
					placas[dados.vagainicial][5] = hora;
					time(&dhSaida);
					calculoHoras[dados.vagainicial][2] = dhSaida;
					
					system("cls");
					printf("Dados de fechamento:\n\n");
				  	printf("\nPlaca: %s", placas[contadorI][1]);
				  	printf("\n\n-------------------------------------\n");
				  	printf("\nData de Entrada: %s", placas[contadorI][2]);
				  	printf("\nHor�rio de Entrada: %s",placas[contadorI][3]);
				  	printf("\n\n");
					printf("-------------------------------------\n");
				  	printf("\nData de Saida: %s",placas[contadorI][4]);
				  	printf("\nHor�rio de Saida: %s",placas[contadorI][5]);
					printf("\n\n");
				  	printf("-------------------------------------\n");
				  	printf("\n\n");
				  	printf("\nTotal devido\n");
				  	printf("\n");
					horaS=3600;
					diferencaIO =(difftime(calculoHoras[dados.vagainicial][2],calculoHoras[dados.vagainicial][1]));
						if (diferencaIO < 3600) {
							valor=dados.horainicial;
						}
						else{											
							resultado=horaS;
							while(resultado<diferencaIO){				// la�o para que leia somente hora (3600)segundos
							 	resultado=resultado+horaS;
							 }
							while(diferencaIO!=resultado){      
								diferencaIO=diferencaIO+1;
							 }
							diferencaIO=diferencaIO-horaS;
							valor = 10 + diferencaIO * dados.horaadicional;
						}
					printf("R$: %.2f \n",valor);
					dados.saldo = dados.saldo + valor; 
			
					//------------------------------------------------- SALVANDO SA�DAS PARA SEREM USADAS EM CASO DE BACKUP
					historicoS = fopen("historicoSaida.txt","a");
						fprintf(historicoS,"%s", placas[contadorI][1]);
						fclose(historicoS);
					  //-------------------------------------------------
				 //*************************************************************************
					for (contadorJ=0;contadorJ<5;contadorJ++){ 						
						placas[contadorI][contadorJ]=" "; //limpando os dados da mem�ria.	
					}
					dados.totalVagasLivres = dados.totalVagasLivres + 1;
				system("pause");
				home();
				 //*************************************************************************
				}
			}
				for(contadorI=0;contadorI<dados.totalvagas;contadorI++){
					if (strcmp(pplaca,placas[contadorI][1])==1){system("cls"); printf("Placa nao encontrada\n\n"); system("pause");home();}
				}    	
        break;
	home();
    case 9:
        system("cls");
        home();
        break;
    default:
        printf("\n\nOpcao nao encontrada!\n\n");
        system("pause");
        system("cls");
        home();
	}

}

//FUN��O PARA CONSULTAS
void consulta(){                        
int contadorI,switchItem;   //vari�veis locais
char placa[8];
char pplaca[8];
system("cls");
    printf("  ESTACIONAMENTO - CONSULTA\n"); //menu consulta
    printf("  1-  PLACA DO VEICULO : \n");
    printf("  2-  VAGAS : \n");
    printf("  9- VOLTAR\n");
    printf("    INFORME A OPCAO: ");
    scanf("%d",&switchItem);
    switch (switchItem){
    case 1:					
		printf(" Digite a placa: ");
		scanf("%s",&placa);
		strupr(placa);
		sprintf(pplaca,"%s",placa);
		system("cls");
		for(contadorI=0;contadorI<dados.totalvagas;contadorI++){ //contador para verificar itens no cadastro
				if(strcmp(pplaca, placas[contadorI][1])==0){	//Se a placa for encontrada...
					printf("\nPlaca: %s", placas[contadorI][1]);//imprimi dados na tela
					printf("\n\n-------------------------------------\n");
					printf("\nData de Entrada: %s", placas[contadorI][2]);
					printf("\nHor�rio de Entrada: %s",placas[contadorI][3]);
					printf("\n\n-------------------------------------\n");
					printf("Aperte qualquer tecla para continuar\n");getch();home();
				}
		}
		for(contadorI=0;contadorI<dados.totalvagas;contadorI++){
				if (strcmp(pplaca, placas[contadorI][1])==1) { //se a placa n�o for encontrada
					system("cls"); 
					printf("Placa nao encontrada\n\n"); 
					printf("Aperte qualquer tecla para continuar\n");getch(); 
					home();
				}
		}	    	
        break;
    case 2:
		system("cls");
			printf("Relatorio de Ocupacao de Vagas:\n\n"); //extrato de ocupa��o do sistema
 	   	    for(contadorI=0;contadorI<dados.totalvagas;contadorI++){ 
			printf("%d ",contadorI);
				   if( (placas[contadorI][1]=="") ||( placas[contadorI][1]==" ") || (placas[contadorI][1]==NULL )){
						printf("Vaga %d - LIVRE\n", contadorI+1);
				   }
				   else {
					    printf("Vaga %d - OCUPADA: %s - Entrada: %s �s %s\n", contadorI+1, placas[contadorI][1],placas[contadorI][2],placas[contadorI][3]);
				   }  
			   } 	
			   printf("Aperte qualquer tecla para continuar\n");getch();
			   system("cls");
			   home();
        break;
    case 9:
        system("cls");
        home();
        break;
	    default:
		system("cls");
	        printf("\n\nOpcao nao encontrada!\n\n");
	        printf("Aperte qualquer tecla para continuar\n");getch();
	        system("cls");
        consulta();
    }
}
//FUN��O PARA MANUTEN��O
void manutencao(){
if(dados.travaSistema==0){
int switchItem, diferenca, contador, condicionalIF, condicionalIF2, contadorJ, whileI, totalVagasC;
char condicionalS, condicionalO;
float horaInicialC, horaAdicionalC;
whileI=0;
system("cls");
	printf("Ola! Se voce for o o responsavel pela manutencao atente\n para essas recomendacoes.Para o correto funcionamento\n do sistema, atualize os valores somente no inicio do expediente.\n Levando em conta que se for alterado durante o funcionamento\n acarretara em erros.\n\n");
	printf("Aperte enter para continuar...");
	getch();
	printf("\n  ESTACIONAMENTO - MANUTENCAO\n");
    	printf("  1-   VALOR HORA INICIAL : \n");
	printf("  2-   VALOR HORA ADICIONAL : \n");
	printf("  3-   TOTAL DE VAGAS : \n");
    	printf("  9- VOLTAR\n");
    	printf("        INFORME A OPCAO: ");
    	scanf("%d",&switchItem);
    	switch (switchItem){
    	case 1: //hora inicial
    		system("cls");
		printf("\n\nValor atual da hora inicial: %.2f.\n Entre com o novo valor: ", dados.horainicial);
        	scanf("%f",&horaInicialC);          //armazena entrada de dados em vari�vel local
        	printf("\nNovo valor: %.2f . Deseja salvar novo valor? s ou n",horaInicialC);
        	condicionalO=getch();fflush(stdin);
        if((condicionalO=='s')||(condicionalO=='S')){
        	dados.horainicial=horaInicialC;     //ap�s confirma��o, armazena valor em vari�vel global.
        	printf("\nDeseja alterar mais algum valor? s ou n ");
        	condicionalIF=getch();fflush(stdin);
	        if((condicionalIF=='s')||(condicionalIF=='S')){
	        	manutencao();
	        	printf("Aperte qualquer tecla para continuar\n");getch();
	        	break;
	        }
	        else
				home();
				break;
			
        }
        else
        	manutencao();
        	break;
        	
    case 2:  //hora adicional
    	system("cls");
        printf("\n\nValor atual da hora adicional: %.2f\n Entre com o novo valor:", dados.horaadicional*3600);
        scanf("%f",&horaAdicionalC);            //os valores s�o divididos ou multiplicados por 3600...
        horaAdicionalC=horaAdicionalC/3600;     //pois o calculo � feito por segundos 
        printf("\nNovo valor: %.2f . Deseja salvar novo valor? s ou n",horaAdicionalC*3600);
        condicionalO=getch();fflush(stdin);
        if((condicionalO=='s')||(condicionalO=='S')){
        	dados.horaadicional=horaAdicionalC;
    		printf("\nDeseja alterar mais algum valor? s ou n ");
    		condicionalIF=getch();fflush(stdin);
	        if((condicionalIF=='s')||(condicionalIF=='S')){
	        	manutencao();
	        	printf("Aperte qualquer tecla para continuar\n");getch();
	        	break;
	        }
	        else
				home();
				break;
        }
        else
        	manutencao();
    		break;
    case 3:  //total de vagas
    	system("cls");
        dados.totalVagasAnterior = dados.totalvagas;
 		printf("\n\nTotal de vagas: %d.\nEntre com o novo valor:", dados.totalvagas);
 		scanf("%d",&totalVagasC);
 		printf("\nNovo valor: %d . Deseja salvar novo valor? s ou n",totalVagasC);
        condicionalO=getch();fflush(stdin);
	        if((condicionalO=='s')||(condicionalO=='S')){
	        	if(totalVagasC>1000) {
					printf("\nLimite maximo de 1000. Ajustando para este limite.");
					dados.totalvagas = 1000;
			 	}
				 	else{
		        		dados.totalvagas=totalVagasC;
		        	}
				diferenca = dados.totalVagasAnterior - dados.totalvagas;
				dados.totalVagasLivres = dados.totalVagasLivres - diferenca; //atualizo o total de vagas livres...
	 			//********reset ************								 //...com base no novo numero de vagas inserido
				for(contador=0;contador<dados.totalvagas;contador++){
			   		for (contadorJ=1;contadorJ<5;contadorJ++)
		  				placas[contador][contadorJ]= " ";
	   	   	   	}
	   	   	   	printf("\nDeseja alterar mais algum valor? s ou n \n");
        		condicionalIF2=getch();fflush(stdin);
	        	if((condicionalIF2=='s')||(condicionalIF2=='S')){
	        		manutencao();
	        		printf("Aperte qualquer tecla para continuar\n");getch();
	        		break;
	        	}
	        	else{
					home();
					break;
				}
	        }
	        if((condicionalO!='s')||(condicionalO!='S')){
	        	manutencao();
	        	break;
	        }
    case 9:
        home();
        break;
    default:
	system("cls");
        printf("\n\nOpcao nao encontrada!\n\n");
        printf("Aperte qualquer tecla para continuar\n");getch();
        system("cls");
        home();
    }
}
else
	system("cls");    //Informa que est� em processo de fechamento de caixa
	puts("\nO Caixa esta sendo fechado. Impossivel modificar dados.");
	printf("Aperte qualquer tecla para continuar\n");getch();
	home();
}
void lerRelatorio(){ //imprime na tela relat�rio
	system("cls");
	printf("\n\n***********************************\n");
	printf("             RELATORIO\n");
	printf("\n");
	printf("O estacionamento recebeu %d veiculo(s)\n", dados.movimentacaodia);
	printf("O saldo e de R$ %.2f ",dados.saldo);
	printf("\n\n***********************************\n");
	printf("Aperte qualquer tecla para continuar\n");getch();
	home();
}
//FUN��O PARA FECHAMENTO DE CAIXA
void fecharCaixa(){
char nomeArquivo[40];
char dataArquivo[20];
char linha[100];
int result;
int result_2;
char *arquivoE; 
char *arquivoS; 
char *arquivoG;
arquivoE="historicoEntrada.txt";
arquivoS="historicoSaida.txt";
arquivoG="historicoGeral.bin";
system("cls");
	printf("\n\n**************************************\n");
	printf("\nO estacionamento recebeu %d veiculo(s)  \n", dados.movimentacaodia);
	printf("\nSaiu/Sairam %d veiculo(s) do estacionamento \n", dados.saidasdia);
	printf("\nPermanece no estacionamento %d veiculo(s)\n",dados.movimentacaodia-dados.saidasdia);
	printf("O saldo e de R$ %.2f \n",dados.saldo);
	printf("\n\n**************************************\n");
	if(dados.movimentacaodia-dados.saidasdia!=0){            //condicional que verifica se ainda h� ve�culos cadastrados
		printf("Ainda existem carros no estacionamento\n Redirecionando para menu principal\n");//se sim imprime isso...
		printf("Aperte qualquer tecla para continuar\n");getch();
		dados.travaSistema=1;                               //e n�o permite acesso ao menu entrada e manuten��o.
		backupGeral();
		home();
	}
	remove(arquivoE);
	remove(arquivoS);
	remove(arquivoG);
	printf("Aperte qualquer tecla para continuar\n");getch();
}
void main(){
int contador, contadorJ;
dados.horainicial=10;
dados.horaadicional=0.0005556;
dados.saldo=0;
dados.movimentacaodia=0;
dados.saidasdia=0;
dados.totalvagas=10;
dados.vagainicial=0;
dados.totalVagasLivres=10;
dados.totalVagasAnterior=0;
dados.travaSistema=0;
	for(contador=0;contador<dados.totalvagas;contador++){
			   		for (contadorJ=1;contadorJ<5;contadorJ++)
		  				placas[contador][contadorJ]= " ";
	   	   	   	}
int v,x,y,z,n;
int achou;
int liberar;
char pplaca[100][8];
char ddata[100][11];
char hhora[100][9];
char pplaca2[100][8];
int result;
struct tempo{
	int ddhEntrada;
}temp[100];
x=0;y=0;z=0;v=0;
historicoE = fopen("historicoEntrada.txt","rt");
historicoS = fopen("historicoSaida.txt","rt");
historicoG = fopen("historicoGeral.bin","a+b");	
//--------------------------------------------------- backup de dados do sistema
	if(historicoG != NULL){
		fread(&dados,sizeof(backup),1,historicoG);
	}
	if(ferror(historicoG)){
		puts("Erro ao abrir arquivo de backup");
		system("pause");
	}
fclose(historicoG);
//----------------------------------------------------- 
//----------------------------------------------------- backup de dados de movimentacao
  if(historicoS != NULL){                        // testa se o arquivo foi aberto com sucesso
		while(z==0){                             // mantem a leitura do arquivo at� ser encontrado EOF
			if(feof(historicoE)){ z=1; }         // condicional que desativa while encontrando EOF
			if(!feof(historicoE)){
				n=0; achou=0;
				fseek(historicoE,v,SEEK_SET);   // posiciona cursor informando o �nicio para leitura.
				fgets(pplaca[x],8,historicoE);  // L� placa
				fgets(ddata[x],11,historicoE);  // L� data
				fgets(hhora[x],9,historicoE);   // L� hora
				fscanf(historicoE,"%d",&temp[x].ddhEntrada);  // L� hora de entrada
				v=v+35;							// C�lculo para posicionar cursor.
				fseek(historicoS,0,SEEK_SET);   
					while(n==0){                // mant�m leitura de arquivo at� EOF.
						if(!feof(historicoS)){  // enquanto o final do arquivo n�o for encontrado...
						fgets(pplaca2[y],8,historicoS);	// L� placa para compara��o
							if(strcmp(pplaca[x],pplaca2[y])!=0){  // Se placas diferentes
								y++;                              // Incrementa contador
							}
							if(strcmp(pplaca[x],pplaca2[y])==0){  // Se placas iguais
								n=1; achou=1; y=0;                // libera la�o while. avisa que encontrou placa igual. Reinicia contador
							}
						}
						else{	n=1;y=0; }                        // Se o final do arquivo foi encontrado libera while e reinicia contador
					}
						if(achou==0){                             // Se n�o foi encontrado placas iguais, faz backup.
							placas[x][1]=pplaca[x];
							placas[x][2]=ddata[x];
							placas[x][3]=hhora[x];
							calculoHoras[x][1]=temp[x].ddhEntrada;
					}
		x++;}
		}	
	}
fclose(historicoE);
fclose(historicoS);	
home();
}
