############################################
#                                          #
#               Pacman  Royal              #
#                                          #
############################################

#Groupe

Prénom:   NOM: 
Bastien   RUIVO 11908153
Mattéo    DERANSART 11914723
Thibault  FAYARD 11906811

#Description

Pacman royal est un battle royal pacman en réseau ou le but est de dévorer les autres pacmans pour qu'il n'en reste qu'un !
	
Version terminal code couleur:

	-Jaune 		: Pacmans (o = sans défense, 0 = a mangé une pacgum)
	-Cyan/Rouge		: Fantomes (si ⋒ vivant et peut vous tuer, si ∞ mort = innofensif)
	-Blanc			: Pacgum (. si mangeable, les fantômes ne pveuvent pas les manger)
	-Vert			: Super Pacgum (ò si mangeable)
	-Bleu			: Couleur des murs, il est impossible de les traverser
	

#Compilation

- make all
	va compiler tout le projet
	
# lancement

- ./bin/debug [SDL ou console] 		va lancer le jeu en solo avec une fenêtre si SDL et dans le terminal si console

- ./bin/server-side 				va lancer une room du serveur et attendre des connection

- ./bin/client-side [ip ex: localhost]	va lancer le client et se connecter au serveur
