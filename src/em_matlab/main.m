[p0,p1,p2,p3,p4,p5,p6,p7,p8,p9] = load_all_patients_func();

model6789 = train(vertcat(p6,p7,p8,p9));

save('model6789.mat','model6789');
