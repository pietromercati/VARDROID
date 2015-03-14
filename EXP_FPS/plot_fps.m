

clear all
close all



b = [0 50000 100000 150000]

for i = 1: length(b)

file_name = strcat('vardroid_', num2str(b(i)));

a = importdata(file_name);

fps(i,1:10) = a(1:10);

end

color = ['r','k','b','g'];

figure
hold on
for i = 1: length(b)
plot(fps(i,:),color(i));
end
hold off
legend ('0','50000','100000','150000');