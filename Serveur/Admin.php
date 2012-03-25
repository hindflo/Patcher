<?php
$user = 'user';
$mdp = '';
$bdd = 'mysql:host=localhost;dbname=bdd';


function afficher_repertoire($repertoire, $BDD, &$i)
{
     if($repertoire != undefined)
            $reponse = $BDD->query("SELECT * FROM POSITION WHERE id_dossier=$repertoire AND (Derniere_position='O' OR Derniere_position = 'T' OR Derniere_position = 'P')");
     else
            $reponse = $BDD->query("SELECT * FROM POSITION WHERE id_dossier IS NULL AND (Derniere_position='O' OR Derniere_position = 'T' OR Derniere_position = 'P')");
     while($donnees = $reponse->fetch())
     {
			$color;
            if($donnees['Derniere_position'] == 'N') $color = 'black';
            else if($donnees['Derniere_position'] == 'T') $color = 'green';
            else if($donnees['Derniere_position'] == 'P') $color = 'red';
            else $color = blue;
            $i++;
            echo "<DIV style='position:relative;height:2px;width:15px;left:45px;background:black;top:12px'></DIV><DIV style='position:relative;left:60px;'>"
            . $donnees['id_Fichier']." : "
            . "<a href='#' onclick=info(". $donnees['id_Fichier'] .",". $donnees['id_Version'] .") style='color:".$color."'>"
            . $donnees['Nom'] . "</a>";
            $j = $i;
            $k = afficher_repertoire($donnees['id_Fichier'], $BDD, $i);
            echo "<DIV style='position:absolute;left:45px;top:20px;width:2px;background:black;display:block;height:". (($k-$j)*22-8) ."'></DIV>"
            . "</DIV>";
     }
     $reponse->closeCursor();
     return $j;
}

$dtext = "que voulez-vous faire?<br>";
$dtext .= "<br>-> <a href=\"#\" onclick=inst(\"init\");>Initialiser la BDD</a><br><br>";
$dtext .= "-> <a href=\"#\" onclick=inst(\"nouvelle\");>Créer une nouvelle version</a><br><br>";
$dtext .= "-> <a href=\"#\" onclick=inst(\"active\");>Activer la version non-active</a><br><br>";
$dtext .= "-> <a href=\"#\" onclick=afficher();>Reprendre la version non-active</a><br><div style='display:inline' id='version'></div>";
$dtext .= "<br>-> <a href=\"#\" onclick=inst(\"supprime\");>Supprimer la version non-active</a><br>";

$arbor = "<br>Ajouter un fichier :"
        . "<TABLE><TR><TD>Nom : </TD><TD><Input id='Nom'></Input></TD></TR>"
        . "<TR><TD>Source : </TD><TD><Input id='Source'></input> Laissez vide pour créer un dossier</TD></TR>"
        . "<TR><TD>Position : </TD><TD><Input id='Posi'></Input> Laissez vide pour le mettre à la racinne</TD></TR></TABLE>"
        . "<Input type=button value=\"Créer\" Onclick=creer()>"
        . "<br><br><TABLE style='display:inline'><TR><TD valign='top'><div id='fichier'>"
        . "<Input type=button value=\"Mettre à jour\" disabled></input><br>"
        . "Id : 0<br>"
        . "Type : ?<br>"
        . "Nom : <Input id='modifier_nom' disabled></input><br>"
        . "Etat : N<br>"
        . "Version de creation : 0<br>"
        . "Version de mise à jour : 0<br>"
        . "Version de suppression : 0<br>"
        . "Source : &nbsp;<Input id='modifier_source' disabled></input><br>"
        . "Position : <Input id='modifier_position' disabled></input><br>"
        . "<Input type=button value=\"Supprimer\" disabled></input><br>"
        . "</div></TD><TD><div style='display:inline' id='arbre'>";

$fin_arbor = "</div></TD></TR></TABLE><br>Légende<br><br>";
if(isset($_GET['Instruction']))
{
    if($_GET['Instruction'] == "init")
    {
        $BDD = new PDO($bdd, $user, $mdp);
        $reponse = $BDD->query("DROP TABLE POSITION");
        $reponse = $BDD->query("DROP TABLE FICHIER");
        $reponse = $BDD->query("DROP TABLE VERSION");
        $reponse = $BDD->query("CREATE TABLE VERSION (id MEDIUMINT PRIMARY KEY AUTO_INCREMENT, Version VARCHAR(20) NOT NULL, Active CHAR(1) NOT NULL CHECK Active IN ('O', 'N'), CHECK(Version <> '0'))");
        $reponse = $BDD->query("CREATE TABLE FICHIER (id MEDIUMINT PRIMARY KEY AUTO_INCREMENT, Version_creation MEDIUMINT NOT NULL REFERENCES VERSION.id, Version_suppression MEDIUMINT REFERENCES VERSION.id, Version_maj MEDIUMINT REFERENCES VERSION.id, Version_futur_maj MEDIUMINT REFERENCES VERSION.id, Source  VARCHAR(200), CHECK(Version_suppression IS NULL OR Version_suppression > Version_creation), CHECK((Version_suppression IS NOT NULL AND Version_maj IS NULL) OR Version_suppression IS NULL), CHECK (Version_maj IS NULL OR Version_maj > Version_creation), CHECK((Source IS NULL AND Version_maj IS NULL AND Version_futur_maj IS NULL) OR Source IS NOT NULL))");
        $reponse = $BDD->query("CREATE TABLE POSITION (id_Fichier MEDIUMINT NOT NULL REFERENCES FICHIER.id, id_Version MEDIUMINT NOT NULL REFERENCES VERSION.id, Nom VARCHAR(100) NOT NULL, id_dossier MEDIUMINT REFERENCES FICHIER.id, Derniere_position CHAR(1) NOT NULL CHECK Derniere_position IN ('O', 'N', 'T', 'P'), PRIMARY KEY(id_Fichier, id_Version), CHECK(id_Fichier <> id_dossier))");
        $reponse = $BDD->query("INSERT INTO VERSION VALUES('', '0', 'O')");
        echo "la BDD a été initialisée, $dtext";
    }
    else if($_GET['Instruction'] == "nouvelle")
    {
        $BDD = new PDO($bdd, $user, $mdp);
        $reponse = $BDD->query("SELECT * FROM VERSION WHERE Active='N'");
        $i = 0;
        while ($donnees = $reponse->fetch())
        {
             $i++;
        }
        $reponse->closeCursor();
        if($i == 0)
        {
            $version = $_GET['version'];
            $reponse = $BDD->query("INSERT INTO VERSION VALUES(NULL, \"$version\", 'N')");
            echo "La nouvelle version a été créée, $dtext";
        }
        else
            echo "Vous ne pouvez pas créer de nouvelles version tant qu'une version est encore en cours de création, $dtext"; 
    }
    else if($_GET['Instruction'] == "active")
    {
        $BDD = new PDO($bdd, $user, $mdp);
        $reponse = $BDD->query("UPDATE VERSION SET Active='O' WHERE Active='N'");
        $reponse = $BDD->query("UPDATE  FICHIER SET Version_maj = Version_futur_maj, Version_futur_maj = NULL WHERE Version_futur_maj IS NOT NULL");
        $reponse = $BDD->query("UPDATE POSITION SET Derniere_position = 'N' WHERE Derniere_position = 'P'");
        $reponse = $BDD->query("UPDATE POSITION SET Derniere_position = 'O' WHERE Derniere_position = 'T'");
        echo "Version activée, $dtext";
    }
    else if($_GET['Instruction'] == "supprime")
    {
        $BDD = new PDO($bdd, $user, $mdp);
        $Reponse = $BDD->query("SELECT id FROM VERSION WHERE Active = 'N'");
        while($donnees = $Reponse->fetch())
        {
        $vers =$donnees['id'];
        $reponse = $BDD->query("DELETE FROM VERSION WHERE id = $vers");
        $reponse = $BDD->query("DELETE FROM POSITION WHERE id_Version = $vers");
        $reponse = $BDD->query("DELETE FROM FICHIER WHERE Version_creation = $vers");
        $reponse = $BDD->query("UPDATE POSITION SET Derniere_position = 'O' WHERE Derniere_position = 'P'");
        $reponse = $BDD->query("UPDATE FICHIER SET Version_suppression = NULL WHERE Version_suppression = $vers");
        $reponse = $BDD->query("UPDATE FICHIER SET Version_futur_maj = NULL WHERE Version_futur_maj = $vers");
        }
        echo "Version supprimée, $dtext";
    }
    else if($_GET['Instruction'] == "afficher")
    {
        $BDD = new PDO($bdd, $user, $mdp);
        $reponse = $BDD->query("SELECT * FROM VERSION WHERE Active='N'");
        $i = 0;
        while ($donnees = $reponse->fetch())
        {
             $i++;
        }
        $reponse->closeCursor();
        if($i == 0)
        {
              echo "<br>Aucune version n'est en cours de création<br>"; return;
        }
        echo $arbor;
        $i = 0;
        afficher_repertoire(undefined, $BDD, $i);
        echo $fin_arbor;
    }
    else if($_GET['Instruction'] == "afficher_rep")
    {
          $BDD = new PDO($bdd, $user, $mdp);
          $i = 0;
          afficher_repertoire(undefined, $BDD, $i);
    }
    else if($_GET['Instruction'] == "creer_fichier")
    {
           $BDD = new PDO($bdd, $user, $mdp);
           $posi = $_GET['position']; $source = $_GET['source'];
           $nom = $_GET['nom'];
           if($posi != '')
           {   
              $reponse = $BDD->query("SELECT * FROM FICHIER WHERE id=$posi AND Source IS NULL");
              $i = 0;
             while ($donnees = $reponse->fetch())
             {
                  $i++;
              }
              $reponse->closeCursor();
              if($i == 0)
              {
                 echo "Le dossier $posi n'existe pas!";
                 return;
              }
           }
           $reponse = $BDD->query("SELECT id FROM VERSION WHERE Active='N'");
           while ($donnees = $reponse->fetch())
           {
                  $version = $donnees['id'];
           }
           $reponse->closeCursor();
           if($source != '')
               $BDD->query("INSERT INTO FICHIER VALUES('', '". $version ."', NULL, NULL, NULL, '". $source ."')");
           else
               $BDD->query("INSERT INTO FICHIER VALUES('', '". $version ."', NULL, NULL, NULL, NULL)");
           $reponse = $BDD->query("SELECT MAX(id) FROM FICHIER");
           $donnees = $reponse->fetch();
           $id = $donnees['MAX(id)'];
           if($posi != '')
               $BDD->query("INSERT INTO POSITION VALUES(". $id .", ". $version .", '". $nom ."', '". $posi ."', 'T')");
           else
               $BDD->query("INSERT INTO POSITION VALUES(". $id .", ". $version .", '". $nom ."', NULL, 'T')");
    }
    else if($_GET['Instruction'] == "info_fichier")
    {
          $BDD = new PDO($bdd, $user, $mdp);
          $version = $_GET['version'];
          $id = $_GET['id'];
          $reponse = $BDD->query("SELECT * FROM FICHIER WHERE id='".$id."'");
          $reponse2 = $BDD->query("SELECT * FROM POSITION WHERE id_Fichier='".$id."' AND id_Version='".$version."'");
          $donnees = $reponse->fetch();
          $donnees2 = $reponse2->fetch();
          $active = "";
          $active2 = "";
          $type = "Fichier";
          if($donnees['Source'] == ''){ $active = "disabled"; $type = "Dossier"; }
          if($donnees2['Derniere_position'] == 'P'){ $active2 = "disabled"; $active = "disabled"; }
          $reponse3 = $BDD->query("SELECT COUNT(*) FROM POSITION WHERE id_dossier='".$id."' AND (Derniere_position='T' OR Derniere_position='O')");   
          $donnees3 = $reponse3->fetch();    
        echo "<Input type=button value=\"Mettre à jour\" onclick=fichier_jour(". $donnees['id'] .",". $donnees2['id_Version'] .") $active></input><br>"
        . "Id : ". $donnees['id'] ."<br>"
        . "Type : " . $type ."<br>"
        . "Nom : <Input id='modifier_nom' value='". $donnees2['Nom'] ."' $active2></input><br>"
        . "Etat : ". $donnees2['Derniere_position'] ."<br>"
        . "Version de creation : ". $donnees['Version_creation'] ."<br>"
        . "Version de mise à jour : ". $donnees['Version_maj'] ."<br>"
        . "Version de suppression : ". $donnees['Version_suppression'] ."<br>"
        . "Source : &nbsp;<Input id='modifier_source' value='". $donnees['Source'] ."' $active></input><br>"
        . "Position : <Input id='modifier_position' value='". $donnees2['id_dossier'] ."' $active2></input><br>";
        if($donnees3['COUNT(*)'] > 0) $active2 = "disabled";
        echo "<Input type=button value=\"Supprimer\" onclick=supprimer_fichier(". $donnees['id'] .",". $donnees2['id_Version'] .") $active2></input><br>";

          $reponse->closeCursor();
          $reponse2->closeCursor();
          $reponse3->closeCursor();
    }
    else if($_GET['Instruction'] == "supprimer_fichier")
    {       
              $BDD = new PDO($bdd, $user, $mdp);
              $reponse = $BDD->query("SELECT id FROM VERSION WHERE Active='N'");
              $donnees = $reponse->fetch();
               $version = $donnees['id'];
              $reponse->closeCursor();
              $fichier = $_GET['id'];
              $i = 0;
              $reponse = $BDD->query("UPDATE FICHIER SET Version_suppression=$version WHERE id=$fichier");
              $reponse = $BDD->query("UPDATE POSITION SET Derniere_position='P' WHERE Derniere_position='O' AND id_Fichier=$fichier");
              $reponse = $BDD->query("DELETE FROM POSITION WHERE id_Version=$version AND id_Fichier=$fichier");
              $reponse = $BDD->query("DELETE FROM FICHIER WHERE Version_creation=$version AND id=$fichier");
              afficher_repertoire(undefined, $BDD, $i);
    }
    else if($_GET['Instruction'] == "fichier_jour")
    {
              $BDD = new PDO($bdd, $user, $mdp);
              $fichier = $_GET['id'];
              $reponse = $BDD->query("SELECT id FROM VERSION WHERE Active='N'");
              $donnees = $reponse->fetch();
               $version = $donnees['id'];
              $reponse->closeCursor();
              $reponse = $BDD->query("UPDATE FICHIER SET Version_futur_maj=$version WHERE id=$fichier");
              $i = 0;
              afficher_repertoire(undefined, $BDD, $i);
    }
    else
    {
        echo "Commande inconnue";
    }
}
else
{
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
?>
<Script>
function inst($instruction)
{
   if($instruction == "init")
   {
         if(!confirm("L'initialisation supprimera toutes vos versions\nÊtes-vous sûr de vouloir initialiser votre BBD?"))
             return;
   }
   else if($instruction == "nouvelle")
   {
          var version = 0;
          while(version == "0")
          {
             version = prompt("Entrez le nom de votre nouvelle version :", "version");
             if(version == undefined)
                 return;
             if(version == "0") alert(version + " est un nom de version interdit!"); 
          }
          $instruction += "&version=" + version;
   }
   else if($instruction == "active")
   {
         if(!confirm("Êtes-vous sûr de vouloir activer la version non-active?"))
             return;
   }
   else if($instruction == "supprime")
   {
         if(!confirm("Êtes-vous sûr de vouloir supprimer la version non-active?"))
             return;
   }
   var xhr = new XMLHttpRequest();
   $url = "http://jsz.kegtux.org/Patcher/Admin.php?Instruction="+$instruction;
   xhr.open('GET', $url);
   xhr.send(null);
   xhr.onreadystatechange = function() {
        if (xhr.readyState == 4 && xhr.status == 200) {
              document.getElementById("corpus").innerHTML = xhr.responseText;
        }
       
   };
}
</Script>
<Script>
function afficher()
{
   var xhr = new XMLHttpRequest();
   $url = "http://jsz.kegtux.org/Patcher/Admin.php?Instruction=afficher";
   xhr.open('GET', $url);
   xhr.send(null);
   xhr.onreadystatechange = function() {
        if (xhr.readyState == 4 && xhr.status == 200) {
              document.getElementById("version").innerHTML = xhr.responseText;
        }
   };
}
</Script>
<Script>
function afficher_arbre()
{
   var xhr = new XMLHttpRequest();
   $url = "http://jsz.kegtux.org/Patcher/Admin.php?Instruction=afficher_rep";
   xhr.open('GET', $url);
   xhr.send(null);
   xhr.onreadystatechange = function() {
        if (xhr.readyState == 4 && xhr.status == 200) {
              document.getElementById("arbre").innerHTML = xhr.responseText;
        }
   };
}
</Script>
<Script>
function creer()
{
    if(document.getElementById('Nom').value == '')
    {
       alert('Vous devez rentrer un nom pour le fichier!');
       return;
    }
   var xhr = new XMLHttpRequest();
   $url = "http://jsz.kegtux.org/Patcher/Admin.php?Instruction=creer_fichier&nom=" + document.getElementById("Nom").value + "&source=" + document.getElementById("Source").value + "&position=" + document.getElementById("Posi").value;
   xhr.open('GET', $url);
   xhr.send(null);
   xhr.onreadystatechange = function() {
        if (xhr.readyState == 4 && xhr.status == 200) {
              if(xhr.responseText != '') alert(xhr.responseText);
              else
                  afficher_arbre();
        }
   };
    
}
</Script>
<Script>
function info($id, $version)
{
   var xhr = new XMLHttpRequest();
   $url = "http://jsz.kegtux.org/Patcher/Admin.php?Instruction=info_fichier&id=" + $id + "&version="+$version;
   xhr.open('GET', $url);
   xhr.send(null);
   xhr.onreadystatechange = function() {
        if (xhr.readyState == 4 && xhr.status == 200) {
              document.getElementById("fichier").innerHTML = xhr.responseText;
        }
   };
}
</Script>
<Script>
function supprimer_fichier($id, $version)
{
   if(!confirm("Êtes-vous sûr de vouloir supprimer ce fichier?")) return;
   var xhr = new XMLHttpRequest();
   $url = "http://jsz.kegtux.org/Patcher/Admin.php?Instruction=supprimer_fichier&id=" + $id;
   xhr.open('GET', $url);
   xhr.send(null);
   xhr.onreadystatechange = function() {
        if (xhr.readyState == 4 && xhr.status == 200) {
              document.getElementById("arbre").innerHTML = xhr.responseText;
              info($id, $version);
        }
   };
}
</Script>
<Script>
function fichier_jour($id, $version)
{
   var xhr = new XMLHttpRequest();
   $url = "http://jsz.kegtux.org/Patcher/Admin.php?Instruction=fichier_jour&id=" + $id;
   xhr.open('GET', $url);
   xhr.send(null);
   xhr.onreadystatechange = function() {
        if (xhr.readyState == 4 && xhr.status == 200) {
              document.getElementById("arbre").innerHTML = xhr.responseText;
              info($id, $version);
        }
   };
}
</Script>
<DIV id="corpus">
<?php echo $dtext; ?>
</DIV>
<?php
}
$bdd = "";
$user = "";
$mdp = "";
$BDD = "";
$donnees = "";
$donnees2= "";
$reponse = "";
$reponse2 = "";
$Reponse = "";
$reponse3 = "";
$donnees3 = "";
?>