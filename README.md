# algebrakart
Algebra Kart

![Algebra Kart](https://github.com/ajaybhaga/MonkeyMaya_com/blob/main/website/content/mm-demo11.gif)

# Algebra Kart: What is it?

- A co-creation time-space
- Real-time networked world - actors, vehicles, physics, interaction
- Driving and vehicle-based physics
- Actor exploration
- Group sound synthesis through drum machines for each networked player and server-side
- Server-side recorder of drum machines - logging music sequence data to database

![Beat module](mayascape_beatmodule.png)



## Development Updates

- Real-time physics world.
- Clients can join server session.
- Server session can spawn AI bot players.
- Bot driving behaviour controlled through Genetic Algorithm implementation with Feed-Forward Neural Network.
- Genotype parameters are used as the weights of the controller network of each bot (Feed-Forward Neural Network).
- After each episode of evolution, the evaluation is calculated to determine a fitness score.
- The fitness score helps in the mutation and cross-over process of genetic evolution.
- The fitness is not always based on a single parameter allowing a more diverse search space for behaviour.
- Vehicle physics for arcade performance driving (artificial forces to assist with terrain or loops)
- Working on Client-side Prediction, improved jerky performance on remote server

IN-PROGRESS:
- Server/client sound synthesis through networked drum sequencers


