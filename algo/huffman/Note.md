# Possibilité de compilations :
- Compilation de la bibliothèque : `make`
- Compilation de l'executable huffman simple : `make huffman`
- Compilation de l'executable huffman de debug : `make debug`

# Utilité 
Le `main.c` ne sert en réalité que de debug pour tester le programme.

# Choix technique
Pour la gestion des codes lors de l'encodage et le décodage de fichier, 3 possibilités s'offrai à nous.
1. Garder la liste classique.
2. Réorganiser la liste pour mettre les codes les plus court en début de liste (car le plus souvent utilisé).
3. Utiliser une table de hashage.

| Choix | Complexité |
|-------|------------|
| 1     | O(n)       |
| 2     | O(n)       |
| 3     | O(1)       |

Nous avons choisi la table de hashage pour sa complexité en O(1) et pour sa simplicité d'utilisation. Nous avons seulement du l'implémenter en plus du projet mais les tables de hashage sont "simple" à implémenter.

Comparaison des temps d'execution entre les 2 méthodes (liste et table de hashage) :
| Choix | Taille du fichier | Temps réel | Temps utilisateur | Temps système |
|-------|-------------------|------------|-------------------|---------------|
| 1 (List) | 7780 | 0m3.026s | 0m0.088s | 0m0.499s |
| | 321 | 0m0.140s | 0m0.000s | 0m0.028s| 
| 2 (Table hashage) | 7780 | 0m0.075s | 0m0.010s | 0m0.000s |
| | 321 | 0m0.053s | 0m0.000s | 0m0.005s |

Comme on peut le voir, la table de hashage est bien plus rapide que la liste.