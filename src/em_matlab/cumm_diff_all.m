cumm_diff_v1 = cumm_diff('010476', 'loosctt2_new.nii');
cumm_diff_v2 = cumm_diff('010769', 'loosctt2_new.nii');
cumm_diff_v3 = cumm_diff('010850', 'loosctt2_new.nii');
cumm_diff_v4 = cumm_diff('010960', 'loosctt2_new.nii');
cumm_diff_v5 = cumm_diff('011030', 'loosctt2_new.nii');

figure1 = figure;
axes1 = axes('Parent',figure1,'YGrid','on','XGrid','on',...
    'Position',[0.0345092838196287 0.124084507042254 0.775 0.815]);
xlim(axes1,[-400 400]);
box(axes1,'on');
hold(axes1,'all');

plot1 = plot(cumm_diff_v1(:,1), cumm_diff_v1(:,2),...
    cumm_diff_v2(:,1), cumm_diff_v2(:,2),...
    cumm_diff_v3(:,1), cumm_diff_v3(:,2),...
    cumm_diff_v4(:,1), cumm_diff_v4(:,2),...
    cumm_diff_v5(:,1), cumm_diff_v5(:,2),...
    'Parent',axes1,'Color',[0 0 0]);
set(plot1(1),'DisplayName','Patient 1');
set(plot1(2),'LineStyle','--','DisplayName','Patient 2');
set(plot1(3),'LineWidth',2,'LineStyle',':','DisplayName','Patient 3');
set(plot1(4),'LineWidth',2,'DisplayName','Patient 4');
set(plot1(5),'LineStyle','-.','DisplayName','Patient 5');

% Create legend
legend1 = legend(axes1,'show');
set(legend1,'EdgeColor',[1 1 1],'Location','NorthWest','YColor',[1 1 1],...
    'XColor',[1 1 1]);