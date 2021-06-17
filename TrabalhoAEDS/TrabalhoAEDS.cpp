#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <speechapi_cxx.h>
#include <locale.h>

#define MAX_NUM 3

using namespace std;
using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;

auto autenticacao = SpeechConfig::FromSubscription("bf12e6f5fe724f31a35adcb4bb74d9d2", "brazilsouth"); //Declaração da autenticação
auto requisicao_textofala = SpeechSynthesizer::FromConfig(autenticacao); //Declaração do objeto de requisição de texto em fala do recurso
auto audio_config = AudioConfig::FromDefaultMicrophoneInput(); //Declaração da entrda do microfone
auto requisicao_falatexto = SpeechRecognizer::FromConfig(autenticacao, audio_config); //Declaração do objeto de requisição de fala em texto do recurso

//Procedimento que requisita da API a transformação de um texto em fala
void texto_em_fala(string Texto) {
    cout << Texto + "\n";
    requisicao_textofala->SpeakTextAsync(Texto).get(); //Requisição da sintetização de texto em fala
}

//Função que requisita da API o reconhecimento de uma fala e a transforma em texto
string fala_em_texto() {
    auto resultado = requisicao_falatexto->RecognizeOnceAsync().get(); //Requisição do reconhecimento de fala em texto
    cout << resultado->Text + "\n";
    return resultado->Text; //Conversão do resultado do reconhecimento em texto
}

int aleatorio() {
    unsigned seed = time(0);
    srand(seed);
    return rand() % 21;
}

int tentativa() {
    int palpite;
    texto_em_fala("\nTente adivinhar o número sorteado entre 0 e 20");
    texto_em_fala("Qual é o numero?");
    string palpite_char = fala_em_texto();
    palpite = stoi(palpite_char);

    if (palpite >= 0 && palpite <= 100)
        return palpite;
    else
        texto_em_fala("O número que você digitou não faz parte do intervalo determinado");
}

int verifica(int gerado, int palpite) {
    if (gerado == palpite)
        return 1;
    else
        return 0;
}

void dica1(int gerado) {
    if (gerado >= 0 && gerado <= 10)
        texto_em_fala("O número está entre 0 e 10");
    else if (gerado > 10 && gerado <= 20)
        texto_em_fala("O número gerado está entre 11 e 20");
}

void dica2(int gerado) {
    if (gerado % 2 == 0)
        texto_em_fala("É par");
    else
        texto_em_fala("É ímpar");
}

void dica3(int gerado, int palpite) {
    if (palpite < gerado)
        texto_em_fala("Você digitou um número menor que o sorteado");
    else
        texto_em_fala("Você digitou um número maior que o gerado");
}

int continuar() {
    int contador = 0;
    texto_em_fala("\Você quer jogar novamente?");
    texto_em_fala("Fale 2 para sair ou 3 para jogar de novo");
    string contador_char = fala_em_texto();
    contador = stoi(contador_char);
    return contador;
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    autenticacao->SetSpeechRecognitionLanguage("pt-BR"); //Configuração da autenticação para o reconhecimento da fala em português
    autenticacao->SetSpeechSynthesisLanguage("pt-BR"); //Configuração da autenticação para a sintetização da fala em português
    autenticacao->SetSpeechSynthesisVoiceName("pt-BR-HeloisaRUS"); //Configuração da autenticação com uma voz específica
    requisicao_textofala = SpeechSynthesizer::FromConfig(autenticacao); //Redefinição do objeto com as novas configurações
    requisicao_falatexto = SpeechRecognizer::FromConfig(autenticacao, audio_config); //Redefinição do objeto com as novas configurações

    try {
        texto_em_fala("Vamos jogar uma partida de adivinhação?");
        int rand, tent, dica;

        do {
            rand = aleatorio();
            do {
                tent = tentativa();
                if (rand != tent) {
                    texto_em_fala("Você errou :( , fale 2 para receber ou repetir as dicas");
                    string dica_char = fala_em_texto();
                    dica = stoi(dica_char);
                    if (dica == 2);
                    dica1(rand);
                    dica2(rand);
                    dica3(rand, tent);
                }
                else{
                    texto_em_fala("Parabéns, você acertou!");
                }
            } while (verifica(rand, tent) != 1);
            system("cls");
        } while (continuar());
    }
    catch (exception fim) {
        cout << "Terminando..." << endl;
    }
}
