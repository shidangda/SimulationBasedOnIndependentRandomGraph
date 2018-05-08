f1=fopen('outfile.txt','r');
result=fscanf(f1,'%f %f',[2,inf]);
x=result(2,:);
y=result(1,:);
plot(x,y);
grid on
xlabel('L/(m*n)值')
ylabel('最大区域所占总区域比例')
title('绝对占优区域出现特点')
