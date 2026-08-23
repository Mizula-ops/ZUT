n = 100;
plansza = 100;
liczba_tur = 200;

osobnicy = zeros(n,8);

for i = 1:n

    osobnicy(i,1) = randi([1 plansza]); % x
    osobnicy(i,2) = randi([1 plansza]); % y
    osobnicy(i,3) = randi([1 3]); % predkosc
    % stan: 1=C, 2=Z, 3=ZD, 4=ZZ
    osobnicy(i,4) = randi([1 4]);
    osobnicy(i,5) = randi([0 60]); % wiek

    if osobnicy(i,5) < 15 || osobnicy(i,5) >= 70
        osobnicy(i,6) = randi([1 3]);      % niska
    elseif osobnicy(i,5) >= 40 && osobnicy(i,5) < 70
        osobnicy(i,6) = randi([4 6]);      % srednia
    elseif osobnicy(i,5) >= 15 && osobnicy(i,5) < 40
        osobnicy(i,6) = randi([7 10]);     % wysoka
    end
    osobnicy(i,7) = rand()*2*pi; 
    if osobnicy(i,4) == 2
    osobnicy(i,8) = randi([0 1]);
    elseif osobnicy(i,4) == 1
        osobnicy(i,8) = randi([0 6]);
    elseif osobnicy(i,4) == 3 
        osobnicy(i,8) = randi([0 4]);
    else                     
        osobnicy(i,8) = 0;
    end
end

for t = 1:liczba_tur

    figure(1)
    clf
    hold on

    scatter(osobnicy(osobnicy(:,4)==1,1), osobnicy(osobnicy(:,4)==1,2), 30,'r','filled')
    scatter(osobnicy(osobnicy(:,4)==2,1), osobnicy(osobnicy(:,4)==2,2), 30,'y','filled')
    scatter(osobnicy(osobnicy(:,4)==3,1), osobnicy(osobnicy(:,4)==3,2), 30,[1 0.5 0],'filled')
    scatter(osobnicy(osobnicy(:,4)==4,1), osobnicy(osobnicy(:,4)==4,2), 30,'g','filled')

    xlim([0 plansza])
    ylim([0 plansza])
    axis square
    drawnow

    for a = 1:size(osobnicy,1)-1
        for b = a+1:size(osobnicy,1)

            d = max(abs(osobnicy(a,1)-osobnicy(b,1)),abs(osobnicy(a,2)-osobnicy(b,2)));

            if d <= 2

                stanA = osobnicy(a,4);
                stanB = osobnicy(b,4);
                % ZZ z Z         
                if stanA == 4 && stanB == 2
                    if osobnicy(a,6) <= 3
                        osobnicy(a,4) = 2;            
                        osobnicy(a,8) = 0;            
                    end
            
                elseif stanA == 2 && stanB == 4           
                    if osobnicy(b,6) <= 3            
                        osobnicy(b,4) = 2;           
                        osobnicy(b,8) = 0;            
                    end
            
                % ZZ z C            
                elseif stanA == 4 && stanB == 1            
                    if osobnicy(a,6) <= 6            
                        osobnicy(a,4) = 2;           
                        osobnicy(a,8) = 0;            
                    else            
                        osobnicy(a,6) = osobnicy(a,6) - 3;            
                    end
            
                elseif stanA == 1 && stanB == 4          
                    if osobnicy(b,6) <= 6          
                        osobnicy(b,4) = 2;          
                        osobnicy(b,8) = 0;         
                    else         
                        osobnicy(b,6) = osobnicy(b,6) - 3;         
                    end
            
                % ZZ z ZD            
                elseif stanA == 4 && stanB == 3            
                    osobnicy(b,6) = osobnicy(b,6) + 1;            
                elseif stanA == 3 && stanB == 4          
                    osobnicy(a,6) = osobnicy(a,6) + 1;

                % ZZ z ZZ            
                elseif stanA == 4 && stanB == 4            
                    thebest = max(osobnicy(a,6), osobnicy(b,6));            
                    osobnicy(a,6) = thebest;          
                    osobnicy(b,6) = thebest;
            
                % C z Z
            
                elseif stanA == 1 && stanB == 2        
                    if osobnicy(b,6) <= 6           
                        osobnicy(b,4) = 1;            
                        osobnicy(b,8) = 0;            
                    end
            
                    osobnicy(a,8) = 0;            
                elseif stanA == 2 && stanB == 1
    
                    if osobnicy(a,6) <= 6         
                        osobnicy(a,4) = 1;           
                        osobnicy(a,8) = 0;
            
                    end
            
                    osobnicy(b,8) = 0;
            
                % C z ZD
            
                elseif stanA == 1 && stanB == 3           
                    if osobnicy(b,6) <= 6           
                        osobnicy(b,4) = 2;          
                        osobnicy(b,8) = 0;          
                    end
            
                elseif stanA == 3 && stanB == 1            
                    if osobnicy(a,6) <= 6            
                        osobnicy(a,4) = 2;            
                        osobnicy(a,8) = 0;            
                    end
            
                % C z C
            
                elseif stanA == 1 && stanB == 1         
                    themin = min(osobnicy(a,6), osobnicy(b,6));          
                    osobnicy(a,6) = themin;         
                    osobnicy(b,6) = themin;          
                    osobnicy(a,8) = 0;         
                    osobnicy(b,8) = 0;
                % Z z ZD
                elseif stanA == 2 && stanB == 3                 
                    osobnicy(b,6) = osobnicy(b,6)-1;
                elseif stanA == 3 && stanB == 2                 
                    osobnicy(a,6) = osobnicy(a,6)-1;
                % Z z Z
                elseif stanA == 2 && stanB == 2                 
                    osobnicy(a,6) = osobnicy(a,6)-1;
                    osobnicy(b,6) = osobnicy(b,6)-1;
                end
                                % narodziny
                if osobnicy(a,5)>=20 && osobnicy(a,5)<=40 && ...
                   osobnicy(b,5)>=20 && osobnicy(b,5)<=40

                    if rand < 0.9
                        ile = randi([1 2]);

                        for k = 1:ile
                            nowy = zeros(1,8);

                            nowy(1) = round((osobnicy(a,1)+osobnicy(b,1))/2);
                            nowy(2) = round((osobnicy(a,2)+osobnicy(b,2))/2);
                            nowy(3) = randi([1 3]);
                            nowy(4) = 4;
                            nowy(5) = 0;
                            nowy(6) = 3;
                            nowy(7) = rand()*2*pi;
                            nowy(8) = 0;

                            osobnicy = [osobnicy; nowy];
                        end
                    end
                end
                osobnicy(a,7) = rand()*2*pi;
                osobnicy(b,7) = rand()*2*pi;

            end

        end

    end
    % wpływ stanu na odporność
    idx = osobnicy(:,4) == 2; % Z
    osobnicy(idx,6) = osobnicy(idx,6) - 0.1;
    
    idx = osobnicy(:,4) == 1; % C
    osobnicy(idx,6) = osobnicy(idx,6) - 0.5;
    
    idx = osobnicy(:,4) == 3; % ZD
    osobnicy(idx,6) = osobnicy(idx,6) + 0.1;
    
    idx = osobnicy(:,4) == 4; % ZZ
    osobnicy(idx,6) = osobnicy(idx,6) + 0.05;
    
    % czas stanu
    osobnicy(:,5)=osobnicy(:,5)+1;
    osobnicy(:,8) = osobnicy(:,8) + 1;
    
    % zmiana stanów po czasie
    idx = osobnicy(:,4) == 2 & osobnicy(:,8) >= 2;
    osobnicy(idx,4) = 1;
    osobnicy(idx,8) = 0;
    
    idx = osobnicy(:,4) == 1 & osobnicy(:,8) >= 7;
    osobnicy(idx,4) = 3;
    osobnicy(idx,8) = 0;
    
    idx = osobnicy(:,4) == 3 & osobnicy(:,8) >= 5;
    osobnicy(idx,4) = 4;
    osobnicy(idx,8) = 0;
    
    for i = 1:size(osobnicy,1)

    if osobnicy(i,5) < 15 || osobnicy(i,5) >= 70
        osobnicy(i,6) = min(osobnicy(i,6), 3);

    elseif osobnicy(i,5) >= 40 && osobnicy(i,5) < 70
        osobnicy(i,6) = min(osobnicy(i,6), 6);

    elseif osobnicy(i,5) >= 15 && osobnicy(i,5) < 40
        osobnicy(i,6) = min(osobnicy(i,6), 10);

    end

    end
    % śmierć
    osobnicy(osobnicy(:,5) >= 100 | osobnicy(:,6) <= 0,:) = [];
  
    % ruch
    osobnicy(:,1) = osobnicy(:,1) + osobnicy(:,3).*cos(osobnicy(:,7));
    osobnicy(:,2) = osobnicy(:,2) + osobnicy(:,3).*sin(osobnicy(:,7));
    
    % odbicie od ścian
    for p = 1:size(osobnicy,1)
    
        if osobnicy(p,1) <= 1 || osobnicy(p,1) >= plansza
            osobnicy(p,7) = pi - osobnicy(p,7);
        end
    
        if osobnicy(p,2) <= 1 || osobnicy(p,2) >= plansza
            osobnicy(p,7) = -osobnicy(p,7);
        end
    
        % zabezpieczenie, żeby nie wyszli poza planszę
        osobnicy(p,1) = min(max(osobnicy(p,1),1),plansza);
        osobnicy(p,2) = min(max(osobnicy(p,2),1),plansza);
    
    end
    pause(0.2)
end
