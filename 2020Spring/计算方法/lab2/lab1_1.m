function error=lab1_1(N)
    i=0:500;
    x=i./50-5;
    f=1./(2+2.*x+x.*x);
    function result=lagrange(N,t)
    h=0:1:N;
    x=-5+10.*h./N;
    y=1./(2+2.*x+x.*x);
    result=0;
    for h=1:N+1
        tmp=1;
        for j=1:h-1
            tmp=tmp*(t-x(j))/(x(h)-x(j));
        end
        for j=h+1:N+1
            tmp=tmp*(t-x(j))/(x(h)-x(j));
        end
        result=result+tmp*y(h);
    end 
    end
    p=0:500;
   % for i=1:500
   %     p(i)=lagrange(N,x(i));
   % end
    %sub=abs(f-p);
    %error=max(sub);
    x;
    printf("%f",x);
end