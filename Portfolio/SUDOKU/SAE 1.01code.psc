procédure grille(char, char, entier);
le programme Sudoku c'est
VIDE= 
début
    [A-I][1-9]=case;
    char X, Y;
    entier chiffre;
    grille (Z, 0, 0); //affiche la grille initial
    tant que (case = VIDE) faire
        écrireEcran("où voulez-vous afficher votre ciffre? :");
        lireClavier("%c %c", X, Y);//X=lettre et Y=chiffre 
        tant que (X!=[A-I] || Y!=[1-9]) faire
            écrireEcran("erreur case inéxistate");
            grille (Z, 0, 0); //affiche la grille
            écrireEcran("où voulez-vous afficher votre ciffre? :");
            lireClavier("%c %c", X, Y);//X=lettre et Y=chiffre 
        finfaire
        écrireEcran("quelle chiffre voullez-vous mettre dans la case %c %c", X, Y);
        lireClavier("%d", chiffre);
        tant que (chiffre!=[1-9]) faire
            écrireEcran("erreur");
            écrireEcran("quelle chiffre voullez-vous mettre dans la case %c %c", X, Y);
            lireClavier("%d", chiffre);
        finfaire
        grille(X, Y, chiffre); //ajoute le chiffre aux coordonnée donné par le joueur et affiche la nouvelle grille (+ verifie si la case est libre et que le chiffre est bien placer)
    finfaire
fin

procédure grille(char a, char b, entier x) c'est
début
    A[1-9]=ligneA;
    B[1-9]=ligneB;
    C[1-9]=ligneC;
    D[1-9]=ligneD;
    E[1-9]=ligneE;
    F[1-9]=ligneF;
    G[1-9]=ligneG;
    H[1-9]=ligneH;
    I[1-9]=ligneI;
    [A-I]1=col1;
    [A-I]2=col2;
    [A-I]3=col3;
    [A-I]4=col4;
    [A-I]5=col5;
    [A-I]6=col6;
    [A-I]7=col7;
    [A-I]8=col8;
    [A-I]9=col9;
    // je ne sais pas comment faire ce code donc je vais l'expliquer en commentaire
    
    // ce code doit affiche la grille du sudoku actuel quant les parametres d'entrée sont Z 0 0

    // il doit aussi changer la grille si les parametre d'entrée sont différent de Z 0 0
    // la grille change en a b par x
    // en verifiant si il n'y a pas deja de x ni dans la ligne a ni dans la colone b

    // exemple ligne:
    si (a='A') faire
        testligneA(x);
    finsi
    // exemple colone:
    si (b='1') faire
        testcol1(x);
    finsi

fin

procédure testligneA(entier y) c'est
début
    
fin

procédure testligneB(entier y) c'est
début
    
fin

procédure testligneC(entier y) c'est
début
    
fin

procédure testligneD(entier y) c'est
début
    
fin

procédure testligneE(entier y) c'est
début
    
fin

procédure testligneF(entier y) c'est
début
    
fin

procédure testligneG(entier y) c'est
début
    
fin

procédure testligneH(entier y) c'est
début
    
fin

procédure testligneI(entier y) c'est
début
    
fin

procédure testcol1(entier y) c'est
début
    
fin

procédure testcol2(entier y) c'est
début
    
fin

procédure testcol3(entier y) c'est
début
    
fin

procédure testcol4(entier y) c'est
début
    
fin

procédure testcol5(entier y) c'est
début
    
fin

procédure testcol6(entier y) c'est
début
    
fin

procédure testcol7(entier y) c'est
début
    
fin

procédure testcol8(entier y) c'est
début
    
fin

procédure testcol9(entier y) c'est
début
    
fin

// je ne suis pas sur des procédure suivante
procédure testcarre1(entier y) c'est
début
    
fin

procédure testcarre2(entier y) c'est
début
    
fin

procédure testcarre3(entier y) c'est
début
    
fin

procédure testcarre4(entier y) c'est
début
    
fin

procédure testcarre5(entier y) c'est
début
    
fin

procédure testcarre6(entier y) c'est
début
    
fin

procédure testcarre7(entier y) c'est
début
    
fin

procédure testcarre8(entier y) c'est
début
    
fin

procédure testcarre9(entier y) c'est
début
    
fin