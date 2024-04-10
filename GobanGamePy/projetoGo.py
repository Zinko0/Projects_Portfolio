
def cria_intersecao(col,lin):
    '''cria_intersecao: str x int → intersecao

    cria_intersecao(col,lin) recebe um caracter e um inteiro correspondentes à
    coluna col e à linha lin e devolve a interseção correspondente. 
    O construtor verifica a validade dos seus argumentos, 
    gerando um ValueError com a mensagem 'cria_intersecao: argumentos invalidos' 
    caso os seus argumentos não sejam válidos.
    '''
    intersecao = (col,lin)
    if(eh_intersecao(intersecao)):
        return intersecao
    raise ValueError('cria_intersecao: argumentos invalidos')

def obtem_col(i):
    '''obtem_col: intersecao → str

    obtem_col(i) devolve a coluna col da interseção
    '''
    return i[0]

def obtem_lin(i):
    '''obtem_lin: intersecao → str

    obtem_lin(i) devolve a linha linha da interseção
    '''
    return i[1]

def eh_intersecao(arg):
    '''eh_intersecao: universal → booleano

    eh_intersecao(arg) devolve True caso o seu argumento seja um TAD intersecao
    e False caso contrário.
    '''
    if(type(arg) == tuple and len(arg) == 2 and type(arg[0]) == str \
        and len(arg[0]) == 1 and 'A' <= arg[0] <= 'S' \
        and type(arg[1]) == int and 1 <= arg[1] <= 19 ): 
        return True
    return False

def intersecoes_iguais(i1, i2):
    '''intersecoes_iguais: universal x universal → booleano

    intersecoes_iguais(i1, i2) devolve True apenas se i1 e i2 forem interseções iguais.
    Devolve False caso contrário.
    '''
    if(eh_intersecao(i1) and eh_intersecao(i2)):
        return obtem_col(i1) == obtem_col(i2) and obtem_lin(i1) == obtem_lin(i2)
    return False

def intersecao_para_str(i):
    '''intersecao_para_str : intersecao → str

    intersecao_para_str(i) devolve a cadeia de caracteres 
    que representa interseção dada.
    '''
    return f"{obtem_col(i)}{obtem_lin(i)}"

def str_para_intersecao(s):
    '''str_para_intersecao: str → intersecao

    str_para_intersecao(s) devolve a interseção representada pelo seu argumento.
    '''
    return cria_intersecao(s[0],int(s[1:]))

def obtem_intersecoes_adjacentes(i, l):
    '''obtem_intersecoes_adjacentes: intersecao x intersecao → tuplo

    obtem_intersecoes_adjacentes(i, l) devolve um tuplo com as interseções adjacentes
    à interseção i de acordo com a ordem de leitura em que l corresponde à interseção
    superior direita do tabuleiro de Go
    '''
    adjacentes = () #verifica se a interseção está nas bordas do tabuleiro
                    #para não gerar erro caso tente criar uma interseção
                    #fora das bordas
    if (not obtem_col(i) == 'A'):
        adjacentes += (cria_intersecao(chr(ord(obtem_col(i)) - 1),obtem_lin(i)),)
    if (not obtem_col(i) == obtem_col(l)):
        adjacentes += (cria_intersecao(chr(ord(obtem_col(i))+ 1),obtem_lin(i)),)
    if (not obtem_lin(i) == 1):
        adjacentes += (cria_intersecao(obtem_col(i),obtem_lin(i)-1),)
    if (not obtem_lin(i) == obtem_lin(l)):
        adjacentes += (cria_intersecao(obtem_col(i),obtem_lin(i)+1),)
    return ordena_intersecoes(adjacentes)

def ordena_intersecoes(t):
    '''ordena_intersecoes: tuplo → tuplo

    ordena_intersecoes(t) recebe um tuplo de interseções
    e devolve um tuplo de interseções com as mesmas interseções
    de t ordenadas de acordo com a ordem de leitura do tabuleiro de Go.
    '''
    def compara(x):
        return obtem_lin(x),obtem_col(x)
    t = tuple(sorted(t,key=compara))
        
    return t

    
def cria_pedra_branca():  #NOTA IMPORTANTE: ver se o cria_pedra_branca
    '''cria_pedra_branca: {} → pedra

    cria_pedra_branca() devolve uma pedra pertencente ao jogador branco.
    '''
    return 'O'

def cria_pedra_preta():
    '''cria_pedra_preta: {} → pedra

    cria_pedra_preta() devolve uma pedra pertencente ao jogador preta.
    '''
    return 'X'
    
def cria_pedra_neutra():
    ''' cria_pedra_neutra: {} → pedra

    cria_pedra_neutra() devolve uma pedra neutra.
    '''
    return '.'
    
def eh_pedra(arg):
    '''eh_pedra: universal → booleano

    eh_pedra(arg) devolve True caso o seu argumento seja um TAD pedra e False
    caso contrário.'''
    if(eh_pedra_branca(arg) or eh_pedra_preta(arg) or eh_pedra_neutra(arg)):
        return True
    return False

def eh_pedra_branca(p):
    '''eh_pedra_branca: pedra → booleano

    eh_pedra_branca(p) devolve True caso a pedra p seja do jogador branco 
    e False caso contrário
    '''
    return p == 'O'

def eh_pedra_preta(p):
    '''eh_pedra_preta: pedra → booleano

    eh_pedra_preta(p) devolve True caso a pedra p seja do jogador preta 
    e False caso contrário
    '''
    return p == 'X'

'''Função extra''' #ver se posso fazer isto assim
def eh_pedra_neutra(p):
    '''eh_pedra_preta: pedra → booleano

    eh_pedra_preta(p) devolve True caso a pedra p seja do jogador preta 
    e False caso contrário
    '''
    return p == '.'

def pedras_iguais(p1,p2): 
    return (eh_pedra_branca(p1) and eh_pedra_branca(p2)) or \
        (eh_pedra_preta(p1) and eh_pedra_preta(p2)) or\
        (eh_pedra_neutra(p1) and eh_pedra_neutra(p2))

def pedra_para_str(p): #ver se é preciso forçar o cria_pedra a ser str
    '''pedra_para_str: pedra → str

    pedra_para_str(p) devolve a cadeia de caracteres que representa o jogador dono
    da pedra, isto é, 'O', 'X'ou '.' para pedras do jogador branco, preto ou neutra
    respetivamente.
    '''
    if(eh_pedra_branca(p)):
        return str(cria_pedra_branca())
    elif(eh_pedra_preta(p)):
        return str(cria_pedra_preta())
    elif(eh_pedra_neutra(p)):
        return str(cria_pedra_neutra())
    
def eh_pedra_jogador(p): 
    '''eh_pedra_jogador : pedra → booleano
    
    eh_pedra_jogador(p) devolve True caso a pedra p seja de um jogador e
    False caso contrário.
    '''
    if(eh_pedra_branca(p) or eh_pedra_preta(p)):
        return True
    return False

def cria_goban_vazio(n): #ver comentários
    '''cria_goban_vazio: int → goban

    cria_goban_vazio(n) devolve um goban de tamanho nxn, sem interseções ocupadas. 
    O construtor verifica a validade do argumento,
    gerando um ValueError com a mensagem 'cria_goban_vazio: argumento invalido' 
    caso os seu argumento não seja válido. 
    '''
    if(n == 9 or n == 13 or n == 19):
        goban = {}
        for lin in range(0,n): #cria um dict com todas as interseções e respetiva pedra
            for col in range(0,n): 
                goban[cria_intersecao(chr(ord('A') + col),lin+1)] = cria_pedra_neutra()
        return goban
    raise ValueError('cria_goban_vazio: argumento invalido')


def cria_goban(n, ib, ip):
    '''cria_goban: int x tuplo x tuplo → goban

    cria_goban(n, ib, ip) devolve um goban de tamanho n x n, com as interseções
    do tuplo ib ocupadas por pedras brancas e as interseções do tuplo ip ocupadas
    por pedras pretas. 
    O construtor verifica a validade dos argumentos, gerando
    um ValueError com a mensagem 'cria_goban: argumentos invalidos'
    caso os seus argumentos não forem válidos.
    '''
    
    if(eh_goban(cria_goban_vazio(n))):
        goban = cria_goban_vazio(n)
        ivistas = ()
        for branca in ib: 
            if(eh_intersecao_valida(goban,branca) and branca not in ivistas):
                goban = coloca_pedra(goban,branca,cria_pedra_branca())
                ivistas += branca #CUIDADO PODE DAR ERRADO
        for preta in ip:
            if(eh_intersecao_valida(goban,preta) and preta not in ivistas):
                goban = coloca_pedra(goban,preta,cria_pedra_preta())    
                ivistas += preta #CUIDADO PODE DAR ERRADO
            else:
                raise ValueError('cria_goban: argumentos invalidos')
        return goban
    raise ValueError('cria_goban: argumentos invalidos')


def cria_copia_goban(t):
    '''cria_copia_goban: goban → goban

    cria_copia_goban(t) recebe um goban e devolve uma cópia do goban
    '''
    copia = t.copy()
    return copia

'''TAD extra'''
def todas_as_intersecoes(g):
    '''todas_as_intersecoes: goban → lista

    todas_as_intersecoes(g) devolve uma lista com todas as interseções do goban
    '''
    return list(g.keys())
''' //////// '''

def obtem_ultima_intersecao(g):
    '''obtem ultima intersecao: goban → intersecao

    obtem_ultima_intersecao(g) devolve a interseção que corresponde ao canto superior direito do goban g
    '''
    t = tuple(todas_as_intersecoes(g))
    return t[-1]


def obtem_pedra(g, i):
    '''obtem_pedra: goban x intersecao → pedra

    obtem pedra(g, i) devolve a pedra na interseção i do goban g. 
    Se a posição não estiver ocupada, devolve uma pedra neutra
    '''
    return g[i]

def obtem_cadeia(g, i):
    '''obtem_cadeia: goban x intersecao → tuplo

    obtem_cadeia(g, i) devolve o tuplo formado pelas interseções 
    das pedras da cadeia que passa pela interseção i. 
    Se a posição não estiver ocupada, devolve a cadeia de posições livres.
    '''
    cadeia = (i,)
    cadeia_anterior = ()
    
    while (cadeia_anterior != cadeia):
        cadeia_anterior = cadeia
        for x in range(len(cadeia)):
            for intersecao in obtem_intersecoes_adjacentes(cadeia[x],obtem_ultima_intersecao(g)):
                if (obtem_pedra(g,i) == obtem_pedra(g,intersecao) and (intersecao not in cadeia)):
                    cadeia += (intersecao,)
        
        
    return ordena_intersecoes(cadeia)


def coloca_pedra(g, i, p):
    '''coloca pedra: goban x intersecao x pedra → goban

    coloca_pedra(g, i, p) modifica destrutivamente o goban g colocando a pedra
    do jogador p na interseção i, e devolve o próprio goban.
    '''
    if(eh_pedra_branca(p)):
        g[i] = cria_pedra_branca()
    if(eh_pedra_preta(p)):
        g[i] = cria_pedra_preta()
    return g

def remove_pedra(g, i):
    '''remove pedra: goban x intersecao → goban

    remove_pedra(g, i) modifica destrutivamente o goban g removendo a pedra
    da interseção i, e devolve o próprio goban.
    '''
    g[i] = cria_pedra_neutra()
    return g


def remove_cadeia(g, t):
    ''' remove cadeia: goban x tuplo → goban

    remove_cadeia(g, t) modifica destrutivamente o goban g removendo as pedras
    nas interseções to tuplo t, e devolve o próprio goban.
    '''
    for intersecao in t:
        remove_pedra(g, intersecao)  #testar para ver se funciona 
    return g


 
def eh_goban(arg): #falta comentar
    '''eh_goban: universal → booleano

    eh_goban(arg) devolve True caso o seu argumento seja um TAD goban e False
    caso contrário.
    '''
    if(type(arg) == dict and (len(todas_as_intersecoes(arg)) == 81 \
        or len(todas_as_intersecoes(arg)) == 169 or len(todas_as_intersecoes(arg)) == 361)):

        for i in list(arg.keys()): 
            if(not eh_intersecao(i)):
                return False
        for p in list(arg.values()):
            if(not eh_pedra(p)):
                return False
        return True
    return False

def eh_intersecao_valida(g, i):
    ''' eh_intersecao_valida: goban x intersecao → booleano

    eh_intersecao_valida(g, i) devolve True se i é uma interseção válida 
    dentro do goban g e False caso contrário.
    '''
    
    if(eh_intersecao(i) \
        and (ord('A') <= ord(obtem_col(i)) <= ord(obtem_col(obtem_ultima_intersecao(g)))) \
        and 1 <= obtem_lin(i) <= obtem_lin(obtem_ultima_intersecao(g))):

        return True
    return False
    
def gobans_iguais(g1, g2):
    '''gobans_iguais: universal x universal → booleano

    gobans_iguais(g1, g2) devolve True apenas se g1 e g2 forem gobans e forem
    iguais.
    '''
    if(eh_goban(g1) and eh_goban(g2)): #ver se é preciso fazer a copia
        return cria_copia_goban(g1) == cria_copia_goban(g2)
    return False

def goban_para_str(g):
    '''goban_para_str: goban → str

    goban_para_str(g) devolve a cadeia de caracteres que representa o goban
    '''
    s = ''
    s_linhas = ''
    s_p_u_linha = '  ' #a primeira e última linha são iguais 
    n = obtem_lin(obtem_ultima_intersecao(g)) 
    pedras = tuple(g.values())
    # ver tambem se devo fazer uma funçao auxiliar
    for l in range(n,0,-1):
        for c in range(n):
            if (l == n):
                s_p_u_linha += ' ' + chr(ord('A') + c)
            s += ' ' + pedra_para_str(pedras[(l-1)*n + c])
        if ((l) >= 10):    #verificar se a linha possui 2 digitos devido aos espaços
            s_linhas += str(l) + s +' '+ str(l) + '\n' # faço cada linha do território (excluindo a primeira e última)
        else:
            s_linhas += ' '+ str(l) + s +'  '+ str(l) + '\n'
        s = '' #apago o s para a próxima linha não ser afetada pela linha anterior
    
    s = s_p_u_linha + '\n' + s_linhas + s_p_u_linha # coloco as linhas do meio entre a primeira e última linha

    return s

def obtem_territorios(g):
    '''obtem_territorios: goban → tuplo

    obtem_territorios(g) devolve o tuplo formado pelos tuplos com as interseções de
    cada território de g. A função devolve as interseções de cada território ordenadas
    em ordem de leitura do tabuleiro de Go, e os territórios ordenados em ordem de
    leitura da primeira interseção do território.
    '''
    territorios = ()
    intersecoes = todas_as_intersecoes(g)  #é uma lista

    for i in intersecoes:
        if(obtem_pedra(g,i) == cria_pedra_neutra()):
            territorios += (obtem_cadeia(g,i),)
            for j in territorios[-1]:
                intersecoes.remove(j)

    return territorios   #supostamente já vem ordenado
        
def obtem_adjacentes_diferentes(g, t):
    '''obtem_adjacentes_diferentes: goban x tuplo → tuplo

    obtem_adjacentes_diferentes(g, t) devolve o tuplo ordenado formado pelas interseções
    adjacentes às interseções do tuplo t,sendo que estas interseções podem ser:

    (a) livres, se as interseções do tuplo t estiverem ocupadas por pedras de jogador;
    (b) ocupadas por pedras de qualquer jogador, se as interseções do tuplo t estiverem livres
    '''
    #supondo que o t já é o obtem_cadeia
    if (obtem_pedra(g,t[0]) == cria_pedra_preta() or \
        obtem_pedra(g,t[0]) == cria_pedra_branca()):   #caso (a)
        liberdades = ()
        for intersecao in t:
            for adjacente in obtem_intersecoes_adjacentes(intersecao,obtem_ultima_intersecao(g)):
                if(obtem_pedra(g,adjacente) == cria_pedra_neutra() and adjacente not in liberdades):
                    liberdades += (adjacente,)
        return ordena_intersecoes(liberdades)
    
    elif(obtem_pedra(g,t[0]) == cria_pedra_neutra()): #case (b)
        fronteiras = ()
        for intersecao in t:
            for adjacente in obtem_intersecoes_adjacentes(intersecao,obtem_ultima_intersecao(g)):
                if(obtem_pedra(g,adjacente) == cria_pedra_preta() or \
                    obtem_pedra(g,adjacente) == cria_pedra_branca() and adjacente not in fronteiras):
                    fronteiras += (adjacente,)
        return ordena_intersecoes(fronteiras)

def jogada(g, i, p):
    '''jogada: goban x intersecao x pedra → goban

    jogada(g, i, p) modifica destrutivamente o goban g colocando a pedra de jogador
    p na interseção i e remove todas as pedras do jogador contrário pertencentes a
    cadeias adjacentes a i sem liberdades, devolvendo o próprio goban.
    '''
    

    coloca_pedra(g,i,p)

    if(p == cria_pedra_branca()):
        for adjacente in obtem_intersecoes_adjacentes(i,obtem_ultima_intersecao(g)):
            if (obtem_pedra(g,adjacente) == cria_pedra_preta() \
                and len(obtem_adjacentes_diferentes(g,obtem_cadeia(g,adjacente))) == 0):
                
                remove_cadeia(g,obtem_cadeia(g,adjacente))
    
    if(p == cria_pedra_preta()):
        for adjacente in obtem_intersecoes_adjacentes(i,obtem_ultima_intersecao(g)):
            if (obtem_pedra(g,adjacente) == cria_pedra_branca() \
                and len(obtem_adjacentes_diferentes(g,obtem_cadeia(g,adjacente))) == 0):
                
                remove_cadeia(g,obtem_cadeia(g,adjacente))

def obtem_pedras_jogadores(g):
    '''obtem_pedras_jogadores: goban → tuplo

    obtem_pedras_jogadores(g) devolve um tuplo de dois inteiros que correspondem ao
    número de interseções ocupadas por pedras do jogador branco e preto, respetivamente.
    '''
    n_pretas = 0
    n_brancas = 0
    for intersecao in todas_as_intersecoes(g):
        if(obtem_pedra(g,intersecao) == cria_pedra_branca()):
            n_brancas += 1
        elif(obtem_pedra(g,intersecao) == cria_pedra_preta()):
            n_pretas += 1
    return (n_brancas,n_pretas)


''' FUNÇÕES ADICIONAIS'''

def calcula_pontos(g):
    '''calcula_pontos: goban → tuplo

    calcula_pontos(g) é uma função auxiliar que recebe um goban 
    e devolve o tuplo de dois inteiros com as pontuações 
    dos jogadores branco e preto, respetivamente.
    '''
    pontos_b = obtem_pedras_jogadores(g)[0]   #isto quebra a abstraçao ???
    pontos_p = obtem_pedras_jogadores(g)[-1]

    if(pontos_b == 0 and pontos_p == 0): #caso especial
        return (0,0)
    

    for territorio in obtem_territorios(g):
        pedras_fronteira = []

        for intersecao in obtem_adjacentes_diferentes(g,territorio):
            pedras_fronteira += [obtem_pedra(g,intersecao),]

        if (len(list(filter(lambda x:x == pedras_fronteira[0] ,pedras_fronteira))) == len(pedras_fronteira)):
            if (pedras_fronteira[0] == cria_pedra_branca()):
                pontos_b += len(territorio)
            elif(pedras_fronteira[0] == cria_pedra_preta()):
                pontos_p += len(territorio)


    return (pontos_b,pontos_p)

def eh_jogada_legal(g, i, p, l):
    '''eh_jogada_legal: goban x intersecao x pedra x goban → booleano 

    eh_jogada_legal(g, i, p, l) é uma função auxiliar que recebe:
    um goban g, uma interseção i, uma pedra de jogador p e um outro goban l.

    Devolve True se a jogada for legal.
    Devolve False caso contrário.
    Não modifica g ou l.
    '''
    
    #intersecao tem de ser vália (eh_intersecao_valida)
    #a pedra nao pode ser jogada no local onde já esteja um pedra
    #jogada(g, i, p)
    
    if(eh_intersecao_valida(g,i) and obtem_pedra(g,i) == cria_pedra_neutra()):
        g_apos = cria_copia_goban(g)
        g_antes = cria_copia_goban(l)

        jogada(g_apos, i, p)
        liberdades = len(obtem_adjacentes_diferentes(g_apos,obtem_cadeia(g_apos,i)))

        #verifica se tem liberdades e se o goban repetiu-se
        if(liberdades != 0 and not gobans_iguais(g_apos,g_antes)): 
            return True
        
    return False


def turno_jogador(g, p, l):
    '''turno jogador: goban x pedra x goban → booleano 

    turno_jogador(g, p, l) é uma função auxiliar que recebe:
    um goban g, uma pedra de jogador p e um outro goban l.

    Oferece a opção de colocar uma pedra sua ou passar o turno. 
    Se o jogador passar, a função devolve False sem modificar os argumentos. 
    Caso contrário, a função devolve True e modifica destrutivamente o tabuleiro g de acordo com a jogada realizada. 
    
    A função repete a mensagem caso a jogada não sejam legal.
    ''' 

    if(p == cria_pedra_branca()):
        turno = input(("Escreva uma intersecao ou 'P' para passar [O]:"))
    elif(p == cria_pedra_preta()):
        turno = input(("Escreva uma intersecao ou 'P' para passar [X]:"))
    
    if(turno == 'P'):
        return False
    
    try:
        if(eh_intersecao_valida(g,str_para_intersecao(turno)) and eh_jogada_legal(g, str_para_intersecao(turno), p, l)):
            jogada(g,str_para_intersecao(turno),p)
            return True
    except ValueError:
        return turno_jogador(g,p,l)
    else:
        return turno_jogador(g,p,l)

def go(n, tb, tn):
    '''go: int x tuple x tuple → booleano

    go(n, tb, tn) permite jogar um jogo completo do Go de dois jogadores. 
    A função recebe:
     n → inteiro correspondente à dimenso do tabuleiro
     tb → tuplo com a representação externa das interseções ocupadas pelas brancas 
     tp → tuplo com a representação externa das interseções ocupadas pelas brancas 

    O jogo termina quando os dois jogadores passam a vez de jogar consecutivamente.

    A função devolve True se o jogador com pedras brancas conseguir ganhar o jogo. 
    Devolve False caso contrário. 
    
    A função gera um ValueError com a mensagem 'go: argumentos invalidos' 
    caso os seus argumentos não sejam válidos.
    '''
    
    try:

        brancas= ()
        pretas = ()
        for i in tb:
            brancas += (intersecao_para_str(i),)

        for j in tn:
            pretas += (intersecao_para_str(j),)

        g = cria_goban(n,brancas,pretas)
        
    except ValueError:
        raise ValueError('go: argumentos invalidos')
    
    alterna = 0
    passou = 0

    t_atual = cria_copia_goban(g)
    t_anterior = cria_copia_goban(g)
    
    while(passou != 2):
            

        if(alterna % 2 == 0):
            p = cria_pedra_preta()
        else:
            p = cria_pedra_branca()
        
        if(p == cria_pedra_preta()):
            passou = 0
            
            

        print("Branco (O) tem", calcula_pontos(t_atual)[0],"pontos")
        print("Preto (X) tem", calcula_pontos(t_atual)[-1],"pontos")
        
        print(goban_para_str(t_atual))


        if(not turno_jogador(t_atual,p,t_anterior)):
            passou += 1

        alterna += 1
        t_anterior = t_atual



    
    return calcula_pontos(t_atual)[0] >= calcula_pontos(t_atual)[-1]
     
go(9,(),())
