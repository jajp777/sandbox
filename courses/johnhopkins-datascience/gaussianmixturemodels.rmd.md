Gaussian Mixture Models
========================================================
author: Daniel Frederico Lins Leite
date: 2017/04/21
autosize: true

Gaussian Mixture Models
========================================================

First we will generate 2000 points from two different multivariate Gaussian distributions.
Then we will generate a 2d histogram of this data and render in 3D the possible approximation of the Gaussian, a result that we could have achieved from a Gaussian Mixture Model.

Code
========================================================


```r
library(plotly)
library('MASS')
simulation.g1 <- mvrnorm(n = 1000, c(0,0), diag(2))
simulation.g2 <- mvrnorm(n = 1000, c(-3,-3), diag(c(0.3,10)))
simulation <- rbind(simulation.g1, simulation.g2)
qtd <- kde2d(simulation[,1], simulation[,2])
```

Slide With Plot
========================================================



```
Error in file(con, "rb") : cannot open the connection
```
