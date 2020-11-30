# Rekkon

Ce projet est un PoC sur la possibilité de faire un boîtier camera (capture de photo et de vidéo) efficace et de qualité sur une base de raspberry pi 4 et caméra HQ.

La résolution est limité par le hardware sur 12MP en photo et 1080p30 en video en enregistrement et 720p30 en preview pour garder la fluidité de la vidéo. 

A l'heure actuelle, le logiciel gère seulement l'enregistrement vidéo et audio dans un fichier MP4. La gestion des résolutions et la prise de photo ne sont pas encore fonctionnelles.

Pour le moment, le projet n'est qu'à l'état de PoC personnel et n'est pas propres notamment niveau architecture, Documentation, tests.
De grands changements logiciels vont arriver :

- [x] Passage de la lib Raspicam modifiée à une gestion intégrée dans le projet pour la capture vidéo et photo.
- [ ] Ajout du support de la photo (Still).
- [ ] Changement d'architecture vers un modèle MVC
- [ ] Passage sur un service de fond sur les principales fonctionnalités pour permettre de créer des différents front end. Notamment sur des appareils distant.
- [ ] Passer la gestion de la preview sur un stream (RMTP?) de faible latence de préférence
- [ ] Découpler le front Qt de la base du service
- [ ] Créer un front sur Android (Kotlin)


# Resources utilisées
## Cross-compilation Linux - Raspberry pi

https://mechatronicsblog.com/cross-compile-and-deploy-qt-5-12-for-raspberry-pi/

## Libs audio (Portaudio / libav (ffmpeg))

- portaudio
  
https://github.com/EddieRingle/portaudio/tree/master/bindings/cpp

- libav
  
https://libav.org/documentation/doxygen/master/index.html

https://github.com/leandromoreira/ffmpeg-libav-tutorial

https://github.com/momarkhan/ffmpeg_pcm_f32le_encoder/blob/master/f32le_transcode.c


## RekkonMMALCamera
https://github.com/Otaze/RekkonMMALCamera