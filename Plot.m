f1=fopen('outfile.txt','r');
result=fscanf(f1,'%f %f',[2,inf]);
x=result(2,:);
y=result(1,:);
plot(x,y);
grid on
xlabel('L/(m*n)ֵ')
ylabel('���������ռ���������')
title('����ռ����������ص�')
