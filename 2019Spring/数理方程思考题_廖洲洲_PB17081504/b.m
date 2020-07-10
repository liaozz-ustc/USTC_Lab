clear;
x=0:0.01:1;
t=0:0.1:10;
[X,T]=meshgrid(x,t);
W=0;
for n=1:1000
    W=W+2.*(1.5).*pi.*(-1).*(n+1)./(((1.5).*pi)^2-(n.*pi)^2).*sin(n.*pi.*T).*sin(n.*pi.*X);
end
surf(X,T,W);
xlabel("x");
ylabel("t");
zlabel("w");