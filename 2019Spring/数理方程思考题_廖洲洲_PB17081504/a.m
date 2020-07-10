x=0:0.01:1;
t=0:0.1:10;
[X,T]=meshgrid(x,t);
W=2.*20.*pi.*(-1).*11./((20.*pi)^2-(10.*pi)^2).*sin(10.*pi.*T).*sin(10.*pi.*X);
surf(X,T,W);
xlabel("x");
ylabel("y");
zlabel("w");