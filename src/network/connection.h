#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#define PORT 8000
#define TAILLE_TAMPON 256

#ifdef _WIN32
	//sous windows, compiler avec l'option -lws2_32
	#include <winsock2.h>
	typedef int socklen_t;
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h> //gethostbyname
#endif

#include <string>
#include <thread>
#include <mutex>
#include <vector>

#include <functional>

// [type]
enum connection_type{
	MESSAGE = 0, // Nouveau message
	NEW_CONNECTION = 1, // demande de connection
	CLOSE_CONNECTION = 2, // Information : personne ayant quittée la room
	NEW_GAME = 3, // défini les informations de création de partie
	INSTRUCTION = 5, // instructions de déplacement utilisées par Game
};
// ![type]

// [Message]
struct Message {
	connection_type type; // détermine le type du message
	std::string corps; // contient le message
};

/// @brief crée et initialise une structure Message
/// @param type: type du message
/// @param msg: corps du message
Message create_message(connection_type type, std::string msg);

/// @brief affiche à la console les détails du message
/// @param msg: Message à afficher
void print_message(Message msg);
// ![Message]


class connection
{
public:
	/// @brief constructeur de connection
	/// @param fdSocket: id d'un socket déjà initialisé
	connection(int fdSocket);

	/// @brief destructeur de connection
	~connection();

	/// @brief définit la fonction a appeller lorsqu'un message est reçu
	/// @details
	/// Lorsqu'un message est reçu et que le mode asynchrone est activé, il sera envoyé par cette fonction
	/// @param callbackFct: fonction déjà bindée
	void setCallback(std::function<void(const Message& msg)> callbackFct)
	{
		_callback = callbackFct;
	}

	/// @brief envoie un Message à la machine distante
	/// @param message: Message à envoyer
	void sendMessage(Message message);


	/// @brief renvois le prochain Message reçu
	/// @details
	/// La fonction est bloquante
	/// Pour éviter tout conflit, la lecture en asynchrone ne doit pas être activée
	Message readMessage();


	/// @brief lance la lecture des Messages en asynchrone
	/// details
	/// Un thread est lancé, celui ci renvois les messages reçus par la fonction définie dans connection::setCallback
	void startReadAsync();


	/// @brief arrète la lecture en asynchrone
	/// @details
	/// termine le thread lancé par connection::startReadAsync
	void stopReadAsync();


protected:
	bool isAsync;
	int m_socket;

	std::thread* m_computeMessage;
	std::thread* tWaitForMessage;
	std::mutex mtxSend;
	std::mutex mtxRecv;

	std::function<void(const Message msg)> _callback;

	/// @brief écoute l'entrée de messages sur la connection en asynchrone
	/// @details
	/// C'est la fonction sur laquelle s'exécute le thread de lecture asynchrone
	void readMessageAsync();

	/// @brief écoute le socket et renvoie la première requette reçue
	/// @details
	/// fonction bloquante, renvoie les requettes reçues une à une
	/// utiliser connection::readMessage et connection::startReadAsync pour la lecture
	bool readOneMessage(Message& msg);
};

const std::vector<std::string> explode(const std::string& s, const char& c);


#endif // CONNECTION_HPP
