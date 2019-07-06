# My Malloc

Pour notre malloc, nous sommes partis sur la méthode du binding.

## Structure en mémoire

Nous avons crée une structure de controle avec un tableau d'indexeur
ainsi que l'index du plus petit block libre ainsi que l'index du plus
grand block libre. Cette structure est placé au début de la section
utilisé par brk/sbrk.

Nous avons donc un tableau d'indexeur représenté par une structure
t_free_block qui va contenir chaque block libre en mémoire en fonction
de sa taille.
Nous stockons ce tableau en dehors de la "zone mobile du malloc"
(espace modifié par brk/sbrk) en utilisant la fonction mmap. Notre
tableau peut contenir des blocks de taille 32 * PAGE_SIZE maximum. Au
delà, nous utilisons mmap pour allouer la taille demander à PAGE_SIZE
près.

En mémoire ce tableau est représenté comme suit :

index	   |  0	      |  1	 |   ...	  |	   max
taille	   |  0 - 16  |  16 - 32 |   ...	  |(max-16) - max|

Nous avons un t_free_block à chaque index :

| t_free_block : 0->16					|
| max(4 octets) | used (4 octets) | meta[] (n octets) 	|

Chaque free_block est alloué grace à mmap et peut être augmenter tant
que mmap ne retourne pas -1 pour la section demandé.
Grace à la propriété used nous pouvons accéder directement au dernier block libre
contenue dans chaque t_free_bloc via meta[used - 1] ce qui facilite et
augmente la rapidité d'accès à une metadonnée.

La "zone mobile" est composé d'une suite de métadonnée et de donnée
stocké.
Cette métadonnée est composé d'un nombre magic qui permet d'indiquer
si la section est libérable via free et d'un attribut taille qui donne
la taille totale du block (métadonnée + donnée).
Chacune de ses métadonnées est stockée dans le tableau de t_free_block
lors d'un appelle de free au niveau de l'atribut meta à l'index
used. On augmente used un fois la métadonné ajouté.

## Le malloc

Le malloc va commencer par vérifier si l'indexeur est initialisé. S'il
ne l'est pas il l'initialise.
Puis il vérifie la taille de mémoire
demander. Soit il utilise mmap si la taille est supérieur à 32 *
PAGE_SIZE sinon il va parcourir la table des block libres.

Lors de la recherche, nous vérifions l'index correxpondant à la taille
mémoire demandé par rapport à nos index de références, min_index et
max_index :
- si l'index est inférieur à min nous utilisons le dernier block
libre situé à min_index que nous divisons en 2.
- si l'index est supérieur à max_index nous augmentons la taille mémoire
à l'aide sbrk d'une taille de 32 * PAGE_SIZE que nous divisons par la suite.
- si l'index se trouve en min_index et max_index nous parcourons le
tableau jusqu'à trouver le block libre le plus petit utilisable.

Nous optimisons en utilisant l'espace mémoire déjà alloué par sbrk et limitant les
appelles qui sont couteux en temps.

Nous avons décidé de ne pas fusionner les espaces mémoires car les
demandes de petits block sont plus fréquents et que la recherche d'un
block libre peut s'avérer très longue quand le nombre de block
conséquent. De plus il faut que les 2 block soit contigue en mémoire
et que les 2 block à fusionner soit de tailler infèrieur à (32 *
PAGE_SIZE / 2). Trop de condition doivent réunis pour que cela
fonctionne ce qui n'est profitable à notre méthode.

# Le free

Le free va commencer par faire vérifier l'adresse (nombre magic valide
- 0xFEEDBEEF - et taille égale à 16n). Si elle est valide, la fonction va modifier
le nombre magic de la métadonné en 0xDEADBEEF puis la placer dans la
table des indexeurs.
