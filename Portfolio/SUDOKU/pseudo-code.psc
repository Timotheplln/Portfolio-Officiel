

procédure ajouterCandidat(entF/sortF laCase : tCase1, entF val:entier) c'est
début
    si (laCase.nbCandidat+1<=9) alors
        laCase.candidat[laCase.nbCandidat+1]=val;
        laCase.nbCandidat++;
    sinon
        écrireEcran("erreur");
    finsi
fin

procédure retirerCandidat(entF/sortF laCase : tCase1, entF val:entier) c'est
début
    i:entier
    x:entier
    pour i de 0 à 9 pas de 1 faire
        si (laCase.candidat[i]==val) alors
            laCase.nbCandidat=laCase.nbCandidat - 1;
            pour j:entier de i à laCase.nbCandidat pas de 1 faire
                laCase.candidat[j]=laCase.candidat[j+1];
            finfaire
        finsi
    finfaire
fin

fonction estCandidat( entF laCase : tCase1,entF val : entier) délivre booléen c'est
début
    x:booléen
    x=faux
    pour i de 0 à 9 pas de 1 faire
        si (laCase.candidat[i]==val) alors
            x=vrai
        finsi
    finfaire
    retourne x
fin

fonction nbCandidats(entF laCase : tCase1) délivre entier c'est
début
    retourne laCase.nbCandidat;
fin

// cas 2

procédure ajouterCandidat(entF/sortF laCase : tCase2, entF val:entier) c'est
début
    laCase.candidat[val]=vrai;
    laCase.nbCandidat++;
fin

procédure retirerCandidat(entF/sortF laCase : tCase2, entF val:entier) c'est
début
    laCase.candidat[val]=faux
    laCase.nbCandidat = laCase.nbCandidat - 1
fin

fonction estCandidat( entF laCase : tCase2,entF val : entier) délivre booléen c'est
début
    retourne laCase.candidat[val];
fin

fonction nbCandidats(entF laCase : tCase2) délivre entier c'est
début
    retourne laCase.nbCandidat;
fin

procédure afficherStats(entF nbCasesVidesInit, entF nbCasesVidesFinal, entF nbCandidatsInit, entF ngCandidatsfinal) c'est
début
    tauxRemplissage: entier;
    tauxRemplissage := ()
fin