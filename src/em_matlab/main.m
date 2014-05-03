[p0,p1,p2,p3,p4,p5,p6,p7,p8,p9] = load_all_patients_func_t2();

model0123 = train(vertcat(p0,p1,p2,p3));

save('t2model0123.mat','model0123');
