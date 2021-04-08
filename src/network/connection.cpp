#include "connection.h"

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <cassert>
//sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>

//TODO: trouver un autre moyen que KILL_LISTENING_THREAD pour terminer la lecture en asynchrone


Message create_message(connection_type typeConnection, std::string txt)
{
	Message msg;
	msg.type = typeConnection;
	msg.corps = txt;
	return msg;
}

void print_message(Message msg)
{
	std::cout<<"******************"<<std::endl;
	std::cout<<"Taille du message : "<< msg.corps.length() <<std::endl;
	std::cout<<"Type de message : " << msg.type << std::endl;
	std::cout << msg.corps << std::endl;
	std::cout<<"******************"<<std::endl;
}

connection::connection(int fdSocket) : isAsync(false), m_socket(fdSocket), tWaitForMessage(nullptr), _callback(nullptr)
{}

connection::~connection()
{
	stopReadAsync();
	close(m_socket);
	m_socket = -1;
}


//=========================
//***** COMMUNICATION *****
//=========================

//requete format : En TETE [Taille message, type du message]
//					Corps Message


void connection::sendMessage(Message message)
{
  mtxSend.lock();
    char requete[TAILLE_TAMPON];


    int longueur =  message.corps.length();
    assert(longueur>0); //Le message doit faire au moins un caractère
    assert(longueur<=TAILLE_TAMPON); // Le message doit faire au plus 256 caractères

    // remplis le message, les requetes doivent être de taille fixe pour éviter l'overlap
    for(int i=longueur; i<TAILLE_TAMPON-2; i++)
    {
		message.corps.push_back('O');
    }


    char taille_message = ((int)longueur)-128-1;
    longueur = snprintf(requete, TAILLE_TAMPON+3, "%c%c%s",
        taille_message, (char)message.type, message.corps.c_str());

    if(send(m_socket, requete, longueur, 0)==-1)
    	std::cerr<<"Tu viens d'envoyer un message dans le vent : "<<errno<<std::endl;

  	mtxSend.unlock();
}

void connection::startReadAsync()
{
    tWaitForMessage = new std::thread(&connection::readMessageAsync, this);
    isAsync = true;
}

void connection::stopReadAsync()
{
	if(isAsync)
	{
		isAsync = false;
		tWaitForMessage->join(); //attend que la dernière requete se soit exécutée
		delete tWaitForMessage;
		tWaitForMessage=nullptr;
	}
}

Message connection::readMessage()
{
	Message msg;

	//gestion des erreurs
	assert(tWaitForMessage==nullptr); // doit être en mode synchrone
	if(!readOneMessage(msg))
	{
		msg=create_message(CLOSE_CONNECTION, "La communication à été interrompue");
	}

	return msg;
}

void connection::readMessageAsync()
{
	Message msg;
	while(readOneMessage(msg))
	{
		// m_computeMessage contient le thread de la dernière requette en date
		if(!isAsync) return;
		// m_computeMessage=new std::thread(_callback, msg);
		// m_computeMessage->join();
		// delete m_computeMessage;
		_callback(msg);
	}

	std::cout<<"mort du thread d'écoute X)"<<std::endl;

	if(_callback!=nullptr)
		_callback(create_message(CLOSE_CONNECTION, "La connection avec le serveur à été perdue"));
}

bool connection::readOneMessage(Message& msg)
{
	char tampon[TAILLE_TAMPON];

	#ifdef _WIN32
		int sd = recv(m_socket, tampon, TAILLE_TAMPON, 0);     /* lecture par bloc */
	#else //Linux
		int sd = read(m_socket, tampon, TAILLE_TAMPON);
	#endif // _WIN32

	if(sd==0 || (connection_type)(int)tampon[1]==KILL_LISTENING_THREAD)
	return false;

	std::string request;
	int taille_requette = tampon[0]+128+1;

	//request.reserve(taille_requette);
	for(int i= 2; i<taille_requette+2; i++)
	{
		request.push_back(tampon[i]);
	}
	msg = create_message((connection_type)(int)tampon[1], request);

	return true;
}
