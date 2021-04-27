#include <stack>
#include <algorithm>
#include <iostream>

#include "Pathfinder.h"
#include "Terrain.h"

struct Pathfinder::Node
{
    int x, y, cost, heur;
};

int Pathfinder::dist(Node n1, Node n2)
{
    return abs(n2.x - n1.x) + abs(n2.y - n1.y);
}

int Pathfinder::compare(Node n1, Node n2)
{
    if (n1.heur < n2.heur)
        return 1;
    else if (n1.heur == n2.heur)
        return 0;
    else
        return -1;
}

void Pathfinder::compute(Node start, Node target, Terrain t)
{
    // Liste des offsets de coordonnées pour retrouver les voisins de la case évaluée à un tour de boucle donné
    int dxy[4][2] = {
        {-1, 0},
        {0, -1},
        {1, 0},
        {0, 1}};

    // Files servant respectivement à énumérer les cases déjà parcourues,
    // les cases à parcourir et les voisins des cases parcourues
    std::deque<Node> closedList, openList, neighborList;
    openList.push_front(start);
    Node u;
    while (!openList.empty())
    {
        u = openList.front(); // /!\ BIEN PENSER A pop_front() UNE FOIS LE TRAITEMENT FINI

        if (u.x == target.x && u.y == target.y)
        {
            //TODO: reconstituerChemin(u); (fonction qui construit la liste d'instructions nécessaires pour atteindre u)
            std::cout << "Algo A* terminé, on reconstruit le chemin maintenant!";
            break;
        }

        // On ajoute les nouveaux voisins à la liste de voisins à traiter
        for (int i = 0; i < 4; i++)
        {
            int gridIndex = (u.y - dxy[i][1]) * t.getHeight() + u.x + dxy[i][0];

            // t.Grille ne renvoie pas d'erreur car Pathfinder est friend de Terrain
            if (t.Grille[gridIndex != '#'])
            {
                Node newNeighbor;
                newNeighbor.x = u.x + dxy[i][0];
                newNeighbor.y = u.y + dxy[i][1];
                neighborList.push_front(newNeighbor);
            }
        }


        // On vérifie si chaque voisin n'existe pas déjà dans nos listes, puis on les valide si ce n'est pas le cas
        for (int j = 0; j < neighborList.size(); j++)
        {
            Node v = neighborList.front();

            // Les deux conditions à ne pas remplir:
            
            // Le voisin v ne doit pas déjà exister dans la liste fermée (la liste des cas traités)
            bool isInClosedList = false;
            // Et il ne doit pas exister dans la liste de cas à traiter avec un coût inférieur
            bool existsWithLesserCostInOpen = false;


            // Test de la première condition
            for(int i = 0; i < closedList.size(); i++)
            {
                Node n = closedList[i];
                if(v.x == n.x and v.y == n.y and v.heur == n.heur and v.cost == n.cost)
                {
                    isInClosedList = true;
                }
            }

            // Test de la deuxième condition
            for(int i = 0; i < openList.size(); i++)
            {
                Node n = openList[i];
                if(v.x == n.x and v.y == n.y and v.heur == n.heur and v.cost > n.cost)
                    existsWithLesserCostInOpen = true;
            }

            // Computation finale, on valide les bons voisins et on supprime l'entrée actuelle de la liste ouverte
            if(!(isInClosedList or existsWithLesserCostInOpen))
            {
                v.cost = u.cost + 1;
                v.heur = v.cost + dist(v, target);
                openList.pop_front();
                openList.push_front(v);
            }
            else
            {
                openList.pop_front();
            }
            closedList.push_front(u);

        }
    }
}