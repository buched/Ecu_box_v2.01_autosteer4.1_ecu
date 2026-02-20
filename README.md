# Ecu_box_v2.01_autosteer4.1

l'utilisation de ce pcb ainsi que tout ce qui s'y rattache (ino, logiciel, config) sont de votre responsabilité. je ne pourrais être tenu responsable dun quelconque problème, bug, erreur de conception..... il en va de votre responsabilité d'effectuer toutes vérifications préalables.

bien placer le pcb dans un endroit ou la température ne monte pas trop, style pas loin du e ventilation 

![4](https://github.com/buched/Ecu_box_v2.01_autosteer4.1_ecu/assets/32975584/601ceffb-bad0-4ff2-9267-419880f9cd88)

![302026551-29ba1a0c-f83b-4070-ab80-7b58118f0af7](https://github.com/buched/Ecu_box_v2.01_autosteer4.1_ecu/assets/32975584/be61849b-fca7-4b01-ab53-0c47f7aac068)

![5](https://github.com/buched/Ecu_box_v2.01_autosteer4.1_ecu/assets/32975584/8fee2c60-2fef-41c3-aafd-2ef68b70a2b7)

exemple de branchement pour moteur électrique 24V :
![motelec](https://github.com/user-attachments/assets/a009cd88-7624-40bc-89eb-0f2b0ca277c7)

Exemple de branchement pour valve Hydraulique : 
<img width="6043" height="1877" alt="hydrauv2" src="https://github.com/user-attachments/assets/b59c3f5a-989c-47b5-9297-5d43912d4de0" />

Boitier :
https://fr.aliexpress.com/item/32857771975.html

convertisseur 3.3v (les modeles vx ou lm fonctionnenet aussi): 
https://www.mouser.fr/ProductDetail/710-173010335

convertisseur 5v (les modeles vx ou lm fonctionnenet aussi): 
https://www.mouser.fr/ProductDetail/710-173010535

câble sma :
https://fr.aliexpress.com/item/1005004690761874.html

câble pour le f9p :
https://fr.aliexpress.com/item/4001175655443.html

condensateur céramiques toujours indispo chez jlcpcb pour le module ethernet 
https://www.tme.eu/fr/details/cch-1n_2000v/condensateurs-ceramiques-tht/sr-passives/

![318373271-3f66ee22-7161-4123-9116-da2f6aa5b637](https://github.com/buched/Ecu_box_v2.01_autosteer4.1_ecu/assets/32975584/72647b8b-1d11-42b8-9340-0c9bf29725ca)

il y a 3 shunt a faire pour switch entre le mode i2c ou RVC du bno (rvc mode pas encore testé)

![318373872-ab552b2e-5fdf-4b62-b678-70731f87bc5f](https://github.com/buched/Ecu_box_v2.01_autosteer4.1_ecu/assets/32975584/b4567608-47a4-4763-9847-f0bdc3fd5ed7)

![3](https://github.com/buched/Ecu_box_v2.01_autosteer4.1_ecu/assets/32975584/e682ae5f-f75a-4f0d-8315-cf73c056a5c1)


branchement du f9p

![1](https://github.com/buched/Ecu_box_v2.01_autosteer4.1_ecu/assets/32975584/e689c696-0e43-45a9-8f29-4c8d4168ac5c)

![2](https://github.com/buched/Ecu_box_v2.01_autosteer4.1_ecu/assets/32975584/85690cf6-3620-4386-8f42-bbcb54efe160)

recepteur gps alternatif UM960/980/982
https://witmotion-sensor.com/products/witmotion-gps-rtk-module-um982-high-precision-centimeter-level-differential-relative-positioning-and-orientation-um960?variant=42873258311877

pour parametrer, injecter le fichier de config joint ici.
![realterm_um980_1](https://github.com/user-attachments/assets/e561fbf4-f5f8-4c48-97d9-0ce7d011f1ff)

![realterm_um980_2](https://github.com/user-attachments/assets/42df4cac-4799-4bd2-8455-09355cc500be)

mini pcb additionnel pour ceux ayant besoin d'une sortie relais
![photo_2024-04-02_18-50-29](https://github.com/buched/Ecu_box_v2.01_autosteer4.1_ecu/assets/32975584/49fc2de2-048a-42ea-912f-3dd5e917dd23)

branchement du bno via le cable pour le positionner à 90 °

![photo_2025-01-18_20-41-53](https://github.com/user-attachments/assets/22112eb9-f91e-42a9-9cb4-3aaeacdc1616)


Tips :

depannage ethernet
sur la centaine de pcb que j'ai vu, c'est le deuxieme qui fini par ne plus fonctionner niveau ethernet. après recherche et tests, il s'avère que les soudures smd de jlcpcb sont limites et le module ethernet fini par se dessouder.

pour vérifier :
tester la continuité entre 1/2 et ensuite entre 5/6 : continuité ok = le problème ne vient pas de ce cas de figure.
si pas de continuité, tester en 3/4 et 7/8 : si continuité, alors le module est légèrement dessoudé.

pour réparer, chauffer le fer et passer rapidement a plat sur tout les contacts et ca devrait suffir

![depannage ethernet](https://github.com/user-attachments/assets/7dcb6623-f1cb-471e-a1a8-a779aa0a1315)

