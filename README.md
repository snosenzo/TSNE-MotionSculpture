#  TSNE Data Motion Sculpture (WIP)

This project is a visualization of the paths that word2vec word vectors take when they are processed by the dimensionality reduction algorithm t-SNE.

For more my thought process and further documentation check out the [wiki](https://github.com/snosenzo/TSNE-MotionSculpture/wiki)

![alt text](https://github.com/snosenzo/TSNE-MotionSculpture/blob/master/bin/data/t3.png "Motion Sculpting")
![alt text](https://github.com/snosenzo/TSNE-MotionSculpture/blob/master/bin/data/t2.png "Motion Sculpting")


![alt text](https://github.com/snosenzo/TSNE-MotionSculpture/blob/master/assets/motionsculpt50.gif "Motion Sculpting")

Above shows the 'sculpting', when the algorithm is processing the data in each frame and moving the points so that there is minimal error between the smaller dimensionality and the larger dimensionality.

![alt text](https://github.com/snosenzo/TSNE-MotionSculpture/blob/master/assets/outsmallerv1.gif "Exploring Animations")

Above shows the result of the algorithm on the default parameters (perplexity = 10, theta = .5)

![alt text](https://github.com/snosenzo/TSNE-MotionSculpture/blob/master/assets/centertsnefin.png "Center of visualization")

At the center of the visualization is usually a set of points that are very similar. I assume that these points are very closely related in meaning as their mapped 3D embedding path is almost the same.

The orbs represent where the points are in the current iteration of the t-SNE algorithm. Then the different colored lines represent the paths of their correspondingly colored spheres. The motion comes from the t-SNE algorithm trying to find the minimum error between the clusters in the higher dimensions and the lower dimension clusters.

To see more 3D t-SNE visualizations: [Tensorflow Embedding Projector](https://projector.tensorflow.org/)

Due to this being a purely visual project first, I cannot verify the accuracy of my program because I have not implemented the ability to see labels of points yet.

 

 
