#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

// DEFININDO FPS E TAMANHO DA TELA
#define _CRT_SECURE_NO_WARNINGS
#define FPS 60.0
#define LARGURA_TELA 1280
#define ALTURA_TELA 720
#define FIM 0
#define MENU 1
#define INICIO 2
#define FORA 3
#define DENTRO 4
#define BATALHA 5
#define INTRUCOES 6
#define INTERACAO 7
#define HISTORIA 8
#define CREDITOS 9
#define RANKING 10
#define GAMEOVER 11
#define VELOCIDADE 6

// INICIANDO PONTEIROS
ALLEGRO_DISPLAY * janela = NULL;
ALLEGRO_FONT* fonte = NULL;
ALLEGRO_FONT* raleway47 = NULL;
ALLEGRO_FONT* raleway30 = NULL;
ALLEGRO_FONT* raleway20 = NULL;
ALLEGRO_FONT* raleway10 = NULL;
ALLEGRO_FONT* ralewayreg = NULL;
ALLEGRO_FONT* lato18 = NULL;
ALLEGRO_EVENT_QUEUE* fila_eventos = NULL;
ALLEGRO_AUDIO_STREAM* ambiente = NULL;
ALLEGRO_AUDIO_STREAM* batalhaMusica = NULL;
ALLEGRO_AUDIO_STREAM* menuMusica = NULL;
ALLEGRO_AUDIO_STREAM* click = NULL;
ALLEGRO_TIMER* timer = NULL;
ALLEGRO_BITMAP* fundo = NULL;
ALLEGRO_BITMAP* fundoPersonagem = NULL;
ALLEGRO_BITMAP* fundoInstru = NULL;
ALLEGRO_BITMAP* dentro = NULL;
ALLEGRO_BITMAP* login = NULL;
ALLEGRO_BITMAP* box = NULL;
ALLEGRO_BITMAP* folha_spriteNpc = NULL;
ALLEGRO_BITMAP* folha_spriteNpc2 = NULL;
ALLEGRO_BITMAP* masculino = NULL;
ALLEGRO_BITMAP* feminino = NULL;
ALLEGRO_BITMAP* status = NULL;
ALLEGRO_BITMAP* fundoBatalha = NULL;
ALLEGRO_BITMAP* prof_kato = NULL;
ALLEGRO_BITMAP* prof_diego = NULL;
ALLEGRO_BITMAP* prof_joice = NULL;
ALLEGRO_BITMAP* game_over = NULL;

int situacaoLado = 0, contFesta = 1;

// STRUCTS
typedef struct {
	int RA;
	char Nome[30];
} Rank;

struct objetos {
	char nome[10];
	float x, y, w, h;
};

typedef struct objetos mapaObjetos;

struct npc {
	int id;
	char nome[20];
	float x, y, w, h;
	char mensagem[500];
};

typedef struct npc Npcsjogo;

struct  player {
	char nome[30];
	int IRA, sexo, sanidade, chaves;
};

typedef struct player Jogador;

Jogador player;
Npcsjogo npc[1];

void inicializaNpcs(float* dirx, float* diry) {
	npc[0].id = 0;
	strcpy(npc[0].nome, "veterana");
	npc[0].x = 1120 + (*dirx + 500); npc[0].y = 1080 + (*diry + 350);
	npc[0].w = 52; npc[0].h = 52;
	strcpy(npc[0].mensagem, "quer ter uma batalha introdutoria?\n");
}

void error_msg(char* text) { // Mostra mensagem de ERRO
	al_show_native_message_box(NULL, "ERRO", "Ocorreu o seguinte erro e o programa sera finalizado:", text, NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

void destroi() {
	if (janela)
		al_destroy_display(janela);
	if (fonte)
		al_destroy_font(fonte);
	if (fila_eventos)
		al_destroy_event_queue(fila_eventos);
	if (batalhaMusica)
		al_destroy_audio_stream(batalhaMusica);
	if (menuMusica)
		al_destroy_audio_stream(menuMusica);
	if (ambiente)
		al_destroy_audio_stream(ambiente);
	if (timer)
		al_destroy_timer(timer);
	if (fundo)
		al_destroy_bitmap(fundo);
	if (login)
		al_destroy_bitmap(login);
	if (box)
		al_destroy_bitmap(box);
	if (folha_spriteNpc)
		al_destroy_bitmap(folha_spriteNpc);
	if (status)
		al_destroy_bitmap(status);

}

int inicializar() {
	if (!al_init()) { // Inicializa o Allegro
		error_msg("Falha ao inicializar a Allegro");
		return 0;
	}

	janela = al_create_display(LARGURA_TELA, ALTURA_TELA); // Cria display
	if (!janela) {
		error_msg("Falha ao criar janela");
		destroi();
		return 0;
	}
	al_set_window_title(janela, "DC - Derrota Continua");

	timer = al_create_timer(1.0 / FPS); // Cria contador
	if (!timer) {
		error_msg("Falha ao criar temporizador");
		destroi();
		return 0;
	}

	al_init_primitives_addon();

	if (!al_install_audio()) { // Inicaliza audio
		error_msg("Falha ao inicializar o audio");
		destroi();
		return 0;
	}

	if (!al_init_acodec_addon()) { // Inicializa codec audio
		error_msg("Falha ao inicializar o codec de audio");
		destroi();
		return 0;
	}

	if (!al_reserve_samples(5)) { // Reserva 5 samples de audio
		error_msg("Falha ao reservar amostrar de audio");
		destroi();
		return 0;
	}

	click = al_load_sample("som/click.ogg");
	if (!click) {
		error_msg("Audio click nao carregado");
		destroi();
		return 0;
	}

	menuMusica = al_load_audio_stream("som/menu.ogg", 4, 1024); // Carrega soundtrack
	if (!menuMusica)
	{
		error_msg("Audio menu nao carregado");
		destroi();
		return 0;
	}

	ambiente = al_load_audio_stream("som/ambiente.ogg", 4, 1024); // Carrega soundtrack
	if (!ambiente)
	{
		error_msg("Audio ambiente nao carregado");
		destroi();
		return 0;
	}

	batalhaMusica = al_load_audio_stream("som/batalha.ogg", 4, 1024); // Carrega soundtrack
	if (!batalhaMusica)
	{
		error_msg("Audio batalha nao carregado");
		destroi();
		return 0;
	}

	al_init_font_addon();
	if (!al_init_ttf_addon()) { // Inicializa add-on de fonte
		error_msg("Falha ao inicializar add-on allegro_ttf");
		destroi();
		return 0;
	}

	if (!al_init_image_addon()) { // Inicializa add-on de imagem
		error_msg("Falha ao inicializar add-on allegro_image");
		destroi();
		return 0;
	}

	if (!al_install_keyboard()) { // Inicializa o teclado
		error_msg("Falha ao inicializar o teclado");
		destroi();
		return 0;
	}

	if (!al_install_mouse()) { // Inicializa o mouse
		error_msg("Falha ao inicializar o mouse");
		destroi();
		return 0;
	}

	if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)) { // Define cursor
		error_msg("Falha ao atribuir ponteiro do mouse");
		destroi();
		return 0;
	}

	raleway47 = al_load_font("fonte/raleway.ttf", 47, 0); //Carregou font raleway tamanho 47
	if (!raleway47) {
		error_msg("Falha ao carregar fonte");
		destroi();
		return 0;
	}

	raleway30 = al_load_font("fonte/raleway.ttf", 30, 0); //Carregou font raleway tamanho 30
	if (!raleway30) {
		error_msg("Falha ao carregar fonte");
		destroi();
		return 0;
	}

	raleway20 = al_load_font("fonte/raleway.ttf", 20, 0); //Carregou font raleway tamanho 20
	if (!raleway20) {
		error_msg("Falha ao carregar fonte");
		destroi();
		return 0;
	}

	raleway10 = al_load_font("fonte/raleway.ttf", 15, 0); //Carregou font raleway tamanho 15
	if (!raleway10) {
		error_msg("Falha ao carregar fonte");
		destroi();
		return 0;
	}

	ralewayreg = al_load_font("fonte/ralewayreg.ttf", 22, 0); //Carregou font raleway regular
	if (!ralewayreg) {
		error_msg("Falha ao carregar fonte");
		destroi();
		return 0;
	}

	lato18 = al_load_font("fonte/lato.ttf", 15, 0); //Carregou font lato 18
	if (!lato18) {
		error_msg("Falha ao carregar fonte");
		destroi();
		return 0;
	}

	// Fila de eventos
	fila_eventos = al_create_event_queue();
	if (!fila_eventos) {
		error_msg("Falha ao criar fila de eventos");
		destroi();
		return 0;
	}
	al_register_event_source(fila_eventos, al_get_display_event_source(janela));
	//al_register_event_source(fila_eventos, al_get_mouse_event_source());
	al_register_event_source(fila_eventos, al_get_keyboard_event_source());
	al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

	// Carrega a folha de sprites na variavel
	folha_spriteNpc = al_load_bitmap("imagem/npc.png");
	if (!folha_spriteNpc) {
		error_msg("Falha ao carregar sprites npc");
		destroi();
		return 0;
	}

	// Usa a cor rosa como transparencia
	al_convert_mask_to_alpha(folha_spriteNpc, al_map_rgb(255, 0, 255));


	// Carrega a folha de sprites do npc2 na variavel
	folha_spriteNpc2 = al_load_bitmap("imagem/npc2.png");
	if (!folha_spriteNpc2) {
		error_msg("Falha ao carregar sprites npc 2");
		destroi();
		return 0;
	}

	// Usa a cor rosa como transparencia
	al_convert_mask_to_alpha(folha_spriteNpc2, al_map_rgb(255, 0, 255));

	// Carrega a folha masculino de sprites na variavel
	masculino = al_load_bitmap("imagem/masculino.png");
	if (!masculino) {
		error_msg("Falha ao carregar sprites");
		destroi();
		return 0;
	}

	// Usa a cor rosa como transparencia
	al_convert_mask_to_alpha(masculino, al_map_rgb(255, 0, 255));

	// Carrega a folha feminino de sprites na variavel
	feminino = al_load_bitmap("imagem/feminino.png");
	if (!feminino) {
		error_msg("Falha ao carregar sprites");
		destroi();
		return 0;
	}

	// Usa a cor rosa como transparencia
	al_convert_mask_to_alpha(feminino, al_map_rgb(255, 0, 255));

	// Fundo
	fundo = al_load_bitmap("imagem/fundoex.bmp");
	if (!fundo) {
		error_msg("Falha ao carregar fundo");
		destroi();
		return 0;
	}

	// Fundo de dentro
	dentro = al_load_bitmap("imagem/dcdentro.bmp");
	if (!dentro) {
		error_msg("Falha ao carregar fundo de dentro");
		destroi();
		return 0;
	}

	// Fundo login
	login = al_load_bitmap("imagem/loginbg.png");
	if (!fundo) {
		error_msg("Falha ao carregar fundo login");
		destroi();
		return 0;
	}

	// Fundo personagem
	fundoPersonagem = al_load_bitmap("imagem/fundopersonagens.png");
	if (!fundoPersonagem) {
		error_msg("Falha ao carregar fundo personagem");
		destroi();
		return 0;
	}

	// Fundo instrucoes
	fundoInstru = al_load_bitmap("imagem/instru.png");
	if (!fundoInstru) {
		error_msg("Falha ao carregar fundo instru��es");
		destroi();
		return 0;
	}

	// Fundo batalha
	fundoBatalha = al_load_bitmap("imagem/salaBatalhaD.jpg");
	if (!fundoBatalha) {
		error_msg("Falha ao carregar fundo batalha");
		destroi();
		return 0;
	}

	// Fundo box
	box = al_load_bitmap("imagem/box.png");
	if (!box) {
		error_msg("Falha ao carregar box");
		destroi();
		return 0;
	}

	// Status
	status = al_load_bitmap("imagem/status.png");
	if (!status) {
		error_msg("Falha ao carregar status box");
		destroi();
		return 0;
	}

	// Katu
	prof_kato = al_load_bitmap("imagem/kato.png");
	if (!prof_kato) {
		error_msg("Falha ao carregar kato");
		destroi();
		return 0;
	}

	// Diogo
	prof_diego = al_load_bitmap("imagem/diego.png");
	if (!prof_diego) {
		error_msg("Falha ao carregar diego");
		destroi();
		return 0;
	}

	// Joice
	prof_joice = al_load_bitmap("imagem/joice.png");
	if (!prof_joice) {
		error_msg("Falha ao carregar joice");
		destroi();
		return 0;
	}

	// Alex
	game_over = al_load_bitmap("imagem/alex.png");
	if (!game_over) {
		error_msg("Falha ao carregar game_over");
		destroi();
		return 0;
	}

	al_start_timer(timer);

	return 0;
}

void HUD() {
	al_draw_filled_rectangle(40, 55, 240, 75, al_map_rgb(64, 64, 64));
	al_draw_filled_rectangle(40, 55, 2 * player.sanidade + 40, 75, al_map_rgb(200, 0, 0));
	al_draw_scaled_bitmap(status, 0, 0, 346, 131, 0, 10, 259.5, 98.25, 0); // BARRA DE STATUS
	al_draw_textf(lato18, al_map_rgb(255, 255, 255), 259 / 2, 26, ALLEGRO_ALIGN_CENTRE, "%s", player.nome);
	al_draw_textf(lato18, al_map_rgb(255, 255, 255), 259 / 2, 53, ALLEGRO_ALIGN_CENTRE, "%d/100", player.sanidade);
	al_draw_textf(lato18, al_map_rgb(255, 255, 255), 259 / 2 - 5, 75, ALLEGRO_ALIGN_LEFT, "%d", player.IRA);
}

void swap(Rank* a, Rank* b) {
	Rank temp;
	strcpy(temp.Nome, a[0].Nome);
	temp.RA = a[0].RA;
	strcpy(a[0].Nome, b[0].Nome);
	a[0].RA = b[0].RA;
	strcpy(b[0].Nome, temp.Nome);
	b[0].RA = temp.RA;
}

void bubbleSort(Rank* v, int n) {
	if (n < 1)return;

	for (int i = 0; i < n; i++)
		if (v[i].RA > v[i + 1].RA)
			swap(&v[i], &v[i + 1]);
	bubbleSort(v, n - 1);
}

Rank* criarranking(Rank* a, int* n) {
	FILE* p;
	char teste[50];
	int ira, pe;
	pe = *n;

	strcpy(teste, player.nome);
	ira = player.IRA;

	if (pe < 10) {
		a[pe].RA = ira;
		strcpy(a[pe].Nome, teste);
		pe = pe + 1;
		bubbleSort(a, pe - 1);
		*n = pe;
	}
	else if (ira > a[0].RA) {
		a[0].RA = ira;
		strcpy(a[0].Nome, teste);
		bubbleSort(a, pe - 1);
	}

	p = fopen("Rank.bin", "wb+");
	fwrite(n, sizeof(int), 1, p);
	fwrite(a, sizeof(Rank), pe, p);
	fclose(p);
	*n = pe;

	return a;
}

void creditos(int* estado) {
	int cor = 0, cont = 0, parte = 0, efeitoCor = 1, cor2 = 50;
	*estado = CREDITOS;
	while (*estado == CREDITOS) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(fila_eventos, &evento);
		if (evento.type == ALLEGRO_EVENT_TIMER) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			if (cor < 255) {
				cor++;
			}
			if (parte == 0) {
				al_draw_multiline_textf(ralewayreg, al_map_rgb(cor, cor, cor), LARGURA_TELA / 2, ALTURA_TELA / 2, 1000, 30, ALLEGRO_ALIGN_CENTRE, "COORDENADOR DE ARTE\n  Fernando Kyoshi Kaida");
			}
			else if (parte == 1) {
				al_draw_multiline_textf(ralewayreg, al_map_rgb(cor, cor, cor), LARGURA_TELA / 2, ALTURA_TELA / 2 - 20, 1000, 30, ALLEGRO_ALIGN_CENTRE, "EQUIPE DE ARTE\n Fernando Kyoshi Kaida\nSara Ferreira");
			}
			else if (parte == 2) {
				al_draw_multiline_textf(ralewayreg, al_map_rgb(cor, cor, cor), LARGURA_TELA / 2, ALTURA_TELA / 2, 1000, 30, ALLEGRO_ALIGN_CENTRE, "CORDENADOR DE PROGRAMACAO\nThiago Araujo Campos.", player.nome);
			}
			else if (parte == 3) {
				al_draw_multiline_textf(ralewayreg, al_map_rgb(cor, cor, cor), LARGURA_TELA / 2, ALTURA_TELA / 2 - 40, 1000, 30, ALLEGRO_ALIGN_CENTRE, "EQUIPE DE PROGRAMACAO\nSara Ferreira\nThiago Araujo Campos\nFernando Kyoshi Kaida");
			}
			else if (parte == 4) {
				al_draw_multiline_textf(ralewayreg, al_map_rgb(cor, cor, cor), LARGURA_TELA / 2, ALTURA_TELA / 2, 1000, 30, ALLEGRO_ALIGN_CENTRE, "CORDENADOR DE ORGANIZACAO E ESCRITA\nSara Ferreira", player.nome);
			}
			else if (parte == 5) {
				al_draw_multiline_textf(ralewayreg, al_map_rgb(cor, cor, cor), LARGURA_TELA / 2, ALTURA_TELA / 2 - 20, 1000, 30, ALLEGRO_ALIGN_CENTRE, "EQUIPE DE ORGANIZACAO E ESCRITA\nSara Ferreira\nThiago Araujo Campos");
			}

			if (efeitoCor) {
				cor2 += 2;
				if (cor2 >= 254) {
					efeitoCor = 0;
				}
			}
			else {
				cor2 -= 2;
				if (cor2 <= 50) {
					efeitoCor = 1;
				}
			}
			al_draw_textf(raleway10, al_map_rgb(cor2, cor2, cor2), LARGURA_TELA - 70, ALTURA_TELA - 85, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
			al_flip_display();
		}
		if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				*estado = FIM;
				break;
			case ALLEGRO_KEY_ENTER:
				*estado = FORA;
				break;
			case ALLEGRO_KEY_SPACE:
				if (parte < 5) {
					parte++;
					cor = 0;
				}
				else {
					*estado = RANKING;
				}
				break;
			}
		}
	}
}

void ranking(int* estado) {
	int altura = 150;
	int qtdP = 0;

	FILE* qtd;
	int* n;
	Rank* ptr;
	n = &qtdP;
	ptr = (Rank*)malloc((11) * sizeof(Rank));
	qtd = fopen("Rank.bin", "rb");


	if (qtd == NULL) {
		fclose(qtd);
		qtd = fopen("Rank.bin", "wb");
		fclose(qtd);
	}
	else {
		fread(&qtdP, sizeof(int), 1, qtd);
		fread(ptr, sizeof(Rank), qtdP, qtd);
		fclose(qtd);
	}

	if (player.IRA != 0) {
		ptr = criarranking(ptr, n);
	}

	while (*estado == RANKING)
	{
		while (!al_event_queue_is_empty(fila_eventos))
		{
			ALLEGRO_EVENT evento;
			al_wait_for_event(fila_eventos, &evento);
			if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
				switch (evento.keyboard.keycode)
				{
				case ALLEGRO_KEY_ESCAPE:
					*estado = MENU;
					break;
				case ALLEGRO_KEY_SPACE:
					*estado = MENU;
					break;
				}
			}
		}
		al_clear_to_color(al_map_rgb(0, 0, 0));

		al_draw_text(raleway30, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 30, ALLEGRO_ALIGN_CENTRE, "RANKING");
		al_draw_text(ralewayreg, al_map_rgb(255, 255, 255), LARGURA_TELA / 2 - 250, 90, ALLEGRO_ALIGN_LEFT, "NOME");
		al_draw_text(ralewayreg, al_map_rgb(255, 255, 255), LARGURA_TELA / 2 + 250, 90, ALLEGRO_ALIGN_RIGHT, "IRA");
		for (int i = qtdP - 1; i >= 0; i--) {
			al_draw_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2 - 250, altura, ALLEGRO_ALIGN_LEFT, "%s", ptr[i].Nome);
			al_draw_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2 + 250, altura, ALLEGRO_ALIGN_RIGHT, "%d", ptr[i].RA);
			altura = (i > 0) ? altura + 50 : 150;
		}
		al_flip_display();
	}
	free(ptr);
}

void instrucoes(int* estado) {
	*estado = INTRUCOES;
	while (*estado == INTRUCOES) {
		al_flip_display();
		ALLEGRO_EVENT evento;
		al_wait_for_event(fila_eventos, &evento);
		if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			*estado = 0;
		}
		if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				*estado = 1;
				break;
			}
		}
		if (evento.type == ALLEGRO_EVENT_TIMER) {
			al_draw_tinted_scaled_rotated_bitmap_region(fundoInstru, 0, 0, 1280, 720, al_map_rgba_f(1, 1, 1, 1), 0, 0, 0, 0, 1, 1, 0, 0);
		}
		al_flip_display();
	}
}

void menu(int* estado) {

	player.IRA = 0;
	player.sanidade = 100;
	situacaoLado = 0;
	player.chaves = 1;
	strcpy(player.nome, "");

	int selecionado = 0;

	al_detach_audio_stream(ambiente);
	al_detach_audio_stream(batalhaMusica);

	al_attach_audio_stream_to_mixer(menuMusica, al_get_default_mixer()); // MIXER DEFAULT
	al_set_audio_stream_playmode(menuMusica, ALLEGRO_PLAYMODE_LOOP); // TOCA EM LOOP
	al_set_mixer_gain(al_get_default_mixer(), 1);
	al_set_audio_stream_gain(menuMusica, 0.2); // Diminui o som de fundo

	while (*estado == MENU) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(fila_eventos, &evento);
		if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			*estado = 0;
		}
		if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				*estado = 0;
				break;
			case ALLEGRO_KEY_DOWN: case ALLEGRO_KEY_S:
				if (selecionado < 3) {
					selecionado++;
					al_play_sample(click, 1.0, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				break;
			case ALLEGRO_KEY_UP: case ALLEGRO_KEY_W:
				if (selecionado > 0) {
					selecionado--;
					al_play_sample(click, 1.0, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				break;
			case ALLEGRO_KEY_ENTER:
				if (selecionado == 3) {
					*estado = FIM;
					al_play_sample(click, 1.0, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else if (selecionado == 2) {
					*estado = RANKING;
					al_play_sample(click, 1.0, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else if (selecionado == 1) {
					instrucoes(estado);
					al_play_sample(click, 1.0, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else if (selecionado == 0) {
					*estado = INICIO;
					al_play_sample(click, 1.0, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				break;
			}
		}

		if (evento.type == ALLEGRO_EVENT_TIMER) {
			al_draw_tinted_scaled_rotated_bitmap_region(login, 0, 0, 1280, 720, al_map_rgba_f(1, 1, 1, 1), 0, 0, 0, 0, 1, 1, 0, 0);
			if (selecionado == 0) {
				al_draw_rounded_rectangle(LARGURA_TELA / 2 - 100, 330, LARGURA_TELA / 2 + 100, 380, 5, 5, al_map_rgb(255, 255, 255), 2); // INICIAR
			}
			else if (selecionado == 1) {
				al_draw_rounded_rectangle(LARGURA_TELA / 2 - 105, 450, LARGURA_TELA / 2 + 105, 495, 5, 5, al_map_rgb(255, 255, 255), 2); // INSTRU��ES
			}
			else if (selecionado == 2) {
				al_draw_rounded_rectangle(LARGURA_TELA / 2 - 105, 505, LARGURA_TELA / 2 + 105, 540, 5, 5, al_map_rgb(255, 255, 255), 2); // RANKING
			}
			else if (selecionado == 3) {
				al_draw_rounded_rectangle(LARGURA_TELA / 2 - 105, 555, LARGURA_TELA / 2 + 105, 590, 5, 5, al_map_rgb(255, 255, 255), 2); // SAIR
			}
		}
		al_flip_display();
	}
}

void historia(int* estado) {
	int cor = 0, cont = 0, parte = 0, efeitoCor = 1, cor2 = 50;
	*estado = 8;
	while (*estado == 8) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(fila_eventos, &evento);
		if (evento.type == ALLEGRO_EVENT_TIMER) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			if (cor < 255) {
				cor++;
			}
			if (parte == 0) {
				al_draw_multiline_textf(ralewayreg, al_map_rgb(cor, cor, cor), LARGURA_TELA / 2, ALTURA_TELA / 2, 1000, 30, ALLEGRO_ALIGN_CENTRE, "Em um mundo onde o DM e o grande terror, um(a) pequeno(a) garoto(a) procura, de forma desesperada, uma salvacao no departamento ao lado: o grande DC.");
			}
			else if (parte == 1) {
				al_draw_multiline_textf(ralewayreg, al_map_rgb(cor, cor, cor), LARGURA_TELA / 2, ALTURA_TELA / 2 - 50, 1000, 30, ALLEGRO_ALIGN_CENTRE, "Contudo, ao chegar proximo do departamento ele(a) entra em uma dimensao estranhamente diferente e percebe que tudo comeca a desafia-lo(a) desenfreadamente.\n Esta na hora, entao, de enfrentar todos os problemas frente a frente e passar em todas as materias que podem o(a) fazer ficar para tras.");
			}
			else if (parte == 2) {
				al_draw_multiline_textf(ralewayreg, al_map_rgb(cor, cor, cor), LARGURA_TELA / 2, ALTURA_TELA / 2, 1000, 30, ALLEGRO_ALIGN_CENTRE, "%s, um(a) nobre guerreiro(a) dos estudos, que anda livremente em seu departamento, busca por esses desafios para derrotar-los e aumentar seu grandioso IRA.", player.nome);
			}
			else if (parte == 3) {
				al_draw_multiline_textf(ralewayreg, al_map_rgb(cor, cor, cor), LARGURA_TELA / 2, ALTURA_TELA / 2 - 60, 1000, 30, ALLEGRO_ALIGN_CENTRE, "%s tem no maximo 9 anos para isso. Contudo, seja rapido(a), tatico(a) e forte para alcancar seu objetivo em menos tempo e garantir sua aprovacao.\n Existe somente uma regra:");
				al_draw_multiline_textf(ralewayreg, al_map_rgb(cor, cor, cor), LARGURA_TELA / 2, ALTURA_TELA / 2 + 80, 1000, 22, ALLEGRO_ALIGN_CENTRE, "NAO DESISTA.");
			}

			if (efeitoCor) {
				cor2 += 2;
				if (cor2 >= 254) {
					efeitoCor = 0;
				}
			}
			else {
				cor2 -= 2;
				if (cor2 <= 50) {
					efeitoCor = 1;
				}
			}
			al_draw_textf(raleway10, al_map_rgb(cor2, cor2, cor2), LARGURA_TELA - 70, ALTURA_TELA - 85, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
			al_flip_display();
		}
		if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				*estado = FIM;
				break;
			case ALLEGRO_KEY_SPACE:
				if (parte < 3) {
					parte++;
					cor = 0;
				}
				else {
					*estado = FORA;
				}
				break;
			}
		}
	}
}

void inicio(int* estado) {
	int situacao = 0, selecionado = 0, personagem = 0;
	char str[30];
	strcpy(str, "");
	while (*estado == INICIO) {
		while (!al_is_event_queue_empty(fila_eventos)) {
			ALLEGRO_EVENT evento;
			al_wait_for_event(fila_eventos, &evento);
			if (evento.type == ALLEGRO_EVENT_KEY_CHAR) {
				if (strlen(str) <= 28 && situacao == 0) {
					char temp[] = { evento.keyboard.unichar, '\0' };
					if (evento.keyboard.unichar == ' ') {
						strcat(str, temp);
					}
					else if (evento.keyboard.unichar >= '0' && evento.keyboard.unichar <= '9') {
						strcat(str, temp);
					}
					else if (evento.keyboard.unichar >= 'A' && evento.keyboard.unichar <= 'Z') {
						strcat(str, temp);
					}
					else if (evento.keyboard.unichar >= 'a' && evento.keyboard.unichar <= 'z') {
						strcat(str, temp);
					}
				}
				if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(str) != 0)
				{
					str[strlen(str) - 1] = '\0';
				}
			}
			if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				*estado = 0;
			}
			if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
				switch (evento.keyboard.keycode) {
				case ALLEGRO_KEY_ESCAPE:
					*estado = 1;
					break;
				case ALLEGRO_KEY_ENTER:
					if (selecionado == 2) {
						*estado = MENU;
					}
					else if (selecionado == 0) {
						situacao = 1;
						selecionado = 1;
						strcpy(player.nome, str);
					}
					else if (selecionado == 1) {
						if (personagem) {
							player.sexo = 1; // Feminino
						}
						else {
							player.sexo = 0; // Masculino
						}
						historia(estado);
					}
					break;
				case ALLEGRO_KEY_DOWN:
					selecionado = 2;
					al_play_sample(click, 1.0, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					break;
				case ALLEGRO_KEY_UP:
					if (situacao == 0) {
						selecionado = 0;
						al_play_sample(click, 1.0, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
					else {
						selecionado = 1;
						al_play_sample(click, 1.0, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
					break;
				case ALLEGRO_KEY_RIGHT:
					if (selecionado == 1) {
						personagem = 1;
						al_play_sample(click, 1.0, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
					break;
				case ALLEGRO_KEY_LEFT:
					if (selecionado == 1) {
						personagem = 0;
						al_play_sample(click, 1.0, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
					break;
				}
			}
			if (evento.type == ALLEGRO_EVENT_TIMER) {
				al_draw_tinted_scaled_rotated_bitmap_region(fundoPersonagem, 0, 0, 1280, 720, al_map_rgba_f(1, 1, 1, 1), 0, 0, 0, 0, 1, 1, 0, 0);
				al_draw_text(raleway30, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 605, ALLEGRO_ALIGN_CENTRE, "Voltar"); // Voltar
				al_draw_text(raleway30, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 220, ALLEGRO_ALIGN_CENTRE, "Digite seu nome:"); // Digite o nome
				al_draw_rectangle(LARGURA_TELA / 2 - 215, 265, LARGURA_TELA / 2 + 215, 300, al_map_rgb(255, 255, 255), 1); // Bloco de texto
				if (strlen(str) > 0) {
					al_draw_text(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 270, ALLEGRO_ALIGN_CENTRE, str);
				}
				if (selecionado == 2) {
					al_draw_rectangle(LARGURA_TELA / 2 - 100, 605, LARGURA_TELA / 2 + 100, 642, al_map_rgb(255, 255, 255), 1); // SAIR SELECIONADO
				}
				if (situacao == 0) {
					al_draw_tinted_scaled_bitmap(masculino, al_map_rgba_f(0.1, 0.1, 0.1, 1), 0, 3 * 1024, 1024, 1024, LARGURA_TELA / 2 - 150 - 64, 380, 128, 128, 0);
					al_draw_tinted_scaled_bitmap(feminino, al_map_rgba_f(0.1, 0.1, 0.1, 1), 0, 3 * 1024, 1024, 1024, LARGURA_TELA / 2 + 150 - 64, 380, 128, 128, 0);
				}
				else if (situacao == 1) {
					if (personagem == 0) {
						al_draw_tinted_scaled_bitmap(masculino, al_map_rgba_f(1, 1, 1, 1), 0, 3 * 1024, 1024, 1024, LARGURA_TELA / 2 - 150 - 64, 380, 128, 128, 0);
						al_draw_tinted_scaled_bitmap(feminino, al_map_rgba_f(0.1, 0.1, 0.1, 1), 0, 3 * 1024, 1024, 1024, LARGURA_TELA / 2 + 150 - 64, 380, 128, 128, 0);
					}
					else {
						al_draw_tinted_scaled_bitmap(masculino, al_map_rgba_f(0.1, 0.1, 0.1, 1), 0, 3 * 1024, 1024, 1024, LARGURA_TELA / 2 - 150 - 64, 380, 128, 128, 0);
						al_draw_tinted_scaled_bitmap(feminino, al_map_rgba_f(1, 1, 1, 1), 0, 3 * 1024, 1024, 1024, LARGURA_TELA / 2 + 150 - 64, 380, 128, 128, 0);
					}
				}
			}
		}
		al_flip_display();
	}
}

void primeiraInteracaoNpc(int* estado) {
	*estado = INTERACAO;
	int efeitoCor = 1, cor = 120, continuar = 1;
	while (*estado == INTERACAO)
	{
		ALLEGRO_EVENT evento;
		al_wait_for_event(fila_eventos, &evento);
		if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				*estado = FORA;
				break;
			case ALLEGRO_KEY_SPACE:
				if (continuar <= 8) continuar++;
				else if (continuar == 9)* estado = FORA;
				break;
			}
		}
		if (evento.type == ALLEGRO_EVENT_TIMER) {
			al_draw_filled_rounded_rectangle(50, 550, 1230, 670, 5, 5, al_map_rgb(17, 17, 28));
			al_draw_textf(ralewayreg, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 555, ALLEGRO_ALIGN_CENTRE, "Veterana");
			if (efeitoCor) {
				cor += 1;
				if (cor >= 255) {
					efeitoCor = 0;
				}
			}
			else {
				cor -= 1;
				if (cor <= 120) {
					efeitoCor = 1;
				}
			}
			al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 85, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");

			if (continuar == 1) {
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 585, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Ola, seja bem-vindx a universidade");
			}
			else if (continuar == 2) {
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 585, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Eu sou sua veterana e quero ajuda-lo a entender como funciona as coisa aqui dentro...");
			}
			else if (continuar == 3) {
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 585, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Primeiramente, e importante lembrar de alguns conceitos:\n IRA, SANIDADE, FESTAS e PROVAS");
			}
			else if (continuar == 4) {
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 585, 1000, 22, ALLEGRO_ALIGN_CENTRE, "O IRA e seu sistema de pontuacao aqui dentro. De acordo com o seu rendimento e desempenho em provas o seu IRA pode reduzir ou aumentar.");
			}
			else if (continuar == 5) {
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 585, 1000, 22, ALLEGRO_ALIGN_CENTRE, "A SANIDADE, no entanto, remete a sua vida aqui. Lembre-se de nunca deixar sua sanidade chegar a zero ou voce pode perder a cabeca e trancar o curso.");
			}
			else if (continuar == 6) {
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 585, 1000, 22, ALLEGRO_ALIGN_CENTRE, "As FESTAS estao extremamente relacionadas a sua sanidade. Festas sao importantes para deixar sua sanidade sempre alta, MAS CUIDADO, pode te afetar diretamente nas provas");
			}
			else if (continuar == 7) {
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 585, 1000, 22, ALLEGRO_ALIGN_CENTRE, "As PROVAS definem quando seu IRA aumenta e sao batalhas disponiveis ao longo do curso que permitem o destrancamento de outras provas. Ir bem nesse ponto significa ter boa pontua�ao.");
			}
			else if (continuar == 7) {
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 585, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Entao...\nAgora que ja expliquei um pouco sobre tudo...");
			}
			else if (continuar == 8) {
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 585, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Ah! Entre no DC para fazer sua primeira materia com o Katu");
			}
			else if (continuar == 9) {
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 585, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Boa sorte em sua graduacao.");
			}
		}
		al_flip_display();
	}
}

void interacaoFesta(int* estado) {
	int selecionado = 1;
	*estado = INTERACAO;

	// Prende no while enquanto estiver interagindo
	while (*estado == INTERACAO)
	{
		ALLEGRO_EVENT evento;
		al_wait_for_event(fila_eventos, &evento);

		// Se uma tecla for pressionada
		if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (evento.keyboard.keycode) // Switch com tecla pressionada
			{
			case ALLEGRO_KEY_D: case ALLEGRO_KEY_RIGHT:
				selecionado = 0;
				break;
			case ALLEGRO_KEY_A: case ALLEGRO_KEY_LEFT:
				selecionado = 1;
				break;
			case ALLEGRO_KEY_ESCAPE:
				*estado = FORA;
			case ALLEGRO_KEY_ENTER:
				if (selecionado && contFesta) {
					if (player.sanidade <= 65) { player.sanidade += 35; }
					else if (player.sanidade > 65) { player.sanidade = 100; }
					player.IRA -= 50;
					*estado = FORA;
					contFesta = 0;
				}
				else {
					*estado = FORA;
					contFesta = 0;
				}
				break;
			}
		}
		if (evento.type == ALLEGRO_EVENT_TIMER) {
			al_draw_tinted_scaled_bitmap(box, al_map_rgba_f(1, 1, 1, 1), 0, 0, 600, 300, LARGURA_TELA / 2 - 150, 100, 300, 150, 0);
			if (contFesta) {
				al_draw_multiline_text(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 120, 300, 30, ALLEGRO_ALIGN_CENTRE, "Voce quer entrar na Festa?\n\nSIM      NAO");
				if (selecionado == 0) {
					al_draw_rectangle(LARGURA_TELA / 2 + 5, 180, LARGURA_TELA / 2 + 62, 205, al_map_rgb(255, 255, 255), 1); // NAO
				}
				else if (selecionado == 1) {
					al_draw_rectangle(LARGURA_TELA / 2 - 62, 180, LARGURA_TELA / 2 - 10, 205, al_map_rgb(255, 255, 255), 1); // SIM
				}
			}
			else {
				al_draw_multiline_text(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 120, 300, 30, ALLEGRO_ALIGN_CENTRE, "A festa ja passou.\nVolte outro dia :)");
			}
		}
		al_flip_display();
	}
}

void interacaoDC(int* estado, float* diry, int* w, int* a, int* d, int* s) {
	int local = *estado, selecionado = 1;
	*estado = INTERACAO;
	while (*estado == INTERACAO)
	{
		ALLEGRO_EVENT evento;
		al_wait_for_event(fila_eventos, &evento);
		if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_D: case ALLEGRO_KEY_RIGHT:
				selecionado = 0;
				break;
			case ALLEGRO_KEY_A: case ALLEGRO_KEY_LEFT:
				selecionado = 1;
				break;
			case ALLEGRO_KEY_ESCAPE:
				if (local == FORA) {
					*estado = FORA;
					*diry -= VELOCIDADE;
					*w = 0; *s = 0; *a = 0; *d = 0;
				}
				else {
					*estado = DENTRO;
					*diry += VELOCIDADE;
					*w = 0; *s = 0; *a = 0; *d = 0;
				}
				break;
			case ALLEGRO_KEY_ENTER:
				if (local == FORA) {
					if (selecionado == 0) {
						*estado = FORA;
						*diry -= VELOCIDADE;
						*w = 0; *s = 0; *a = 0; *d = 0;
					}
					else if (selecionado == 1) {
						*estado = DENTRO;
					}
				}
				else {
					if (selecionado == 0) {
						*estado = DENTRO;
						*diry += VELOCIDADE;
						*w = 0; *s = 0; *a = 0; *d = 0;
					}
					else if (selecionado == 1) {
						*estado = FORA;
					}
				}

				break;
			}
		}
		if (evento.type == ALLEGRO_EVENT_TIMER) {
			al_draw_tinted_scaled_bitmap(box, al_map_rgba_f(1, 1, 1, 1), 0, 0, 600, 300, LARGURA_TELA / 2 - 150, 100, 300, 150, 0);
			if (local == FORA) {
				al_draw_multiline_text(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 120, 300, 30, ALLEGRO_ALIGN_CENTRE, "Voce quer entrar no DC?\n\nSIM      NAO");
			}
			else {
				al_draw_multiline_text(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 120, 300, 30, ALLEGRO_ALIGN_CENTRE, "Voce quer sair no DC?\n\nSIM      NAO");
			}
			if (selecionado == 0) {
				al_draw_rectangle(LARGURA_TELA / 2 + 5, 180, LARGURA_TELA / 2 + 62, 205, al_map_rgb(255, 255, 255), 1); // NAO
			}
			else if (selecionado == 1) {
				al_draw_rectangle(LARGURA_TELA / 2 - 62, 180, LARGURA_TELA / 2 - 10, 205, al_map_rgb(255, 255, 255), 1); // SIM
			}
		}
		al_flip_display();
	}
}

void interacaoBOSS(int* estado, float* diry, int* w, int* a, int* d, int* s, int bloqueia) {
	int local = *estado, selecionado = 1;
	*estado = INTERACAO;
	while (*estado == INTERACAO)
	{
		ALLEGRO_EVENT evento;
		al_wait_for_event(fila_eventos, &evento);
		if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_D: case ALLEGRO_KEY_RIGHT:
				selecionado = 0;
				break;
			case ALLEGRO_KEY_A: case ALLEGRO_KEY_LEFT:
				selecionado = 1;
				break;
			case ALLEGRO_KEY_ESCAPE: case ALLEGRO_KEY_SPACE:
				*estado = DENTRO;
				*diry -= VELOCIDADE;
				*w = 0; *s = 0; *a = 0; *d = 0;
				break;
			case ALLEGRO_KEY_ENTER:
				if (selecionado == 0 && bloqueia == 0) {
					*estado = DENTRO;
					*diry -= VELOCIDADE;
					*w = 0; *s = 0; *a = 0; *d = 0;
				}
				else if (selecionado == 1 && bloqueia == 0) {
					*estado = BATALHA;
				}
				break;
			}
		}
		if (evento.type == ALLEGRO_EVENT_TIMER) {
			if (bloqueia == 0) {
				al_draw_tinted_scaled_bitmap(box, al_map_rgba_f(1, 1, 1, 1), 0, 0, 600, 300, LARGURA_TELA / 2 - 250, 100, 500, 150, 0);
				al_draw_multiline_text(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 120, 300, 30, ALLEGRO_ALIGN_CENTRE, "Voce quer iniciar uma batalha?\n\nSIM      NAO");

				if (selecionado == 0) {
					al_draw_rectangle(LARGURA_TELA / 2 + 5, 180, LARGURA_TELA / 2 + 62, 205, al_map_rgb(255, 255, 255), 1); // NAO
				}
				else if (selecionado == 1) {
					al_draw_rectangle(LARGURA_TELA / 2 - 62, 180, LARGURA_TELA / 2 - 10, 205, al_map_rgb(255, 255, 255), 1); // SIM
				}
			}
			else {
				al_draw_tinted_scaled_bitmap(box, al_map_rgba_f(1, 1, 1, 1), 0, 0, 600, 300, LARGURA_TELA / 2 - 250, 100, 500, 150, 0);
				al_draw_multiline_text(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 120, 300, 30, ALLEGRO_ALIGN_CENTRE, "A materia esta trancada ou ja foi feita\n\nPressione ESPACO para sair");
			}
		}
		al_flip_display();
	}
}

int colisao(mapaObjetos objetos[], float* diry, float* dirx, int nObjetos) {
	int cont = 0;
	for (int i = 0; i < nObjetos; i++) {
		if ((*diry <= objetos[i].y + objetos[i].h || *diry >= objetos[i].y || *dirx >= objetos[i].x || *dirx <= objetos[i].x + objetos[i].w)) {
			cont++;
		}
	}
	if (cont == nObjetos) {
		return 1;
	}
	else {
		return 0;
	}
}

void andandoFora(mapaObjetos objetos[], float* diry, float* dirx, int w, int a, int s, int d, int* regiao_y_folha) {
	if (*dirx >= objetos[0].x - VELOCIDADE) {
		if (d && (*dirx >= -2150 && colisao(objetos, diry, dirx, 1))) {
			*dirx -= VELOCIDADE;
			*regiao_y_folha = 0 * 1024;
		}
		if (w && *diry <= -75) {
			*diry += VELOCIDADE;
			*regiao_y_folha = 2 * 1024;
		}
		if (s && *diry >= -1650) {
			*diry -= VELOCIDADE;
			*regiao_y_folha = 3 * 1024;
		}
		if (a && *dirx <= -5) {
			*dirx += VELOCIDADE;
			*regiao_y_folha = 1 * 1024;
		}
	}
	else if (*dirx <= objetos[0].x + objetos[0].w + VELOCIDADE) {
		if (d && *dirx >= -2150) {
			*dirx -= VELOCIDADE;
			*regiao_y_folha = 0 * 1024;
		}
		if (w && *diry <= -75) {
			*diry += VELOCIDADE;
			*regiao_y_folha = 2 * 1024;
		}
		if (s && *diry >= -1650) {
			*diry -= VELOCIDADE;
			*regiao_y_folha = 3 * 1024;
		}
		if (a && (*dirx <= -5 && colisao(objetos, diry, dirx, 1))) {
			*dirx += VELOCIDADE;
			*regiao_y_folha = 1 * 1024;
		}
	}
	else if (*diry >= objetos[0].y - VELOCIDADE) {
		if (d && *dirx >= -2150) {
			*dirx -= VELOCIDADE;
			*regiao_y_folha = 0 * 1024;
		}
		if (w && *diry <= -75) {
			*diry += VELOCIDADE;
			*regiao_y_folha = 2 * 1024;
		}
		if (s && *diry >= -1650 && colisao(objetos, diry, dirx, 1)) {
			*diry -= VELOCIDADE;
			*regiao_y_folha = 3 * 1024;
		}
		if (a && *dirx <= -5) {
			*dirx += VELOCIDADE;
			*regiao_y_folha = 1 * 1024;
		}
	}
	else if (*diry <= objetos[0].y + objetos[0].h + VELOCIDADE) {
		if (d && *dirx >= -2150) {
			*dirx -= VELOCIDADE;
			*regiao_y_folha = 0 * 1024;
		}
		if (w && *diry <= -75 && colisao(objetos, diry, dirx, 1)) {
			*diry += VELOCIDADE;
			*regiao_y_folha = 2 * 1024;
		}
		if (s && *diry >= -1650) {
			*diry -= VELOCIDADE;
			*regiao_y_folha = 3 * 1024;
		}
		if (a && *dirx <= -5) {
			*dirx += VELOCIDADE;
			*regiao_y_folha = 1 * 1024;
		}
	}

}

void andando(float* diry, float* dirx, int w, int a, int s, int d, int* regiao_y_folha) {
	if (d && *dirx >= -2150) {
		*dirx -= VELOCIDADE;
		*regiao_y_folha = 0 * 1024;
	}
	if (w && *diry <= -75) {
		*diry += VELOCIDADE;
		*regiao_y_folha = 2 * 1024;
	}
	if (s && *diry >= -1650) {
		*diry -= VELOCIDADE;
		*regiao_y_folha = 3 * 1024;
	}
	if (a && *dirx <= -5) {
		*dirx += VELOCIDADE;
		*regiao_y_folha = 1 * 1024;
	}

}

void gameover(int* estado) {
	int cor = 0, cont = 0, efeitoCor = 1, cor2 = 50;
	*estado = GAMEOVER;
	while (*estado == GAMEOVER) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(fila_eventos, &evento);
		if (evento.type == ALLEGRO_EVENT_TIMER) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			if (cor < 255) {
				cor++;
			}
			al_draw_tinted_scaled_bitmap(game_over, al_map_rgb(cor, cor, cor), 0, 0, 1024, 934, LARGURA_TELA / 2 - 256, ALTURA_TELA / 2 - 100 - 233.5, 512, 467, 0);
			al_draw_multiline_textf(ralewayreg, al_map_rgb(cor, cor, cor), LARGURA_TELA / 2, ALTURA_TELA / 2 + 100, 1000, 30, ALLEGRO_ALIGN_CENTRE, "GAME-OVER.");
			if (efeitoCor) {
				cor2 += 2;
				if (cor2 >= 254) {
					efeitoCor = 0;
				}
			}
			else {
				cor2 -= 2;
				if (cor2 <= 50) {
					efeitoCor = 1;
				}
			}
			player.IRA = 0;
			player.sanidade = 100;
			situacaoLado = 0;
			player.chaves = 1;
			al_draw_textf(raleway10, al_map_rgb(cor2, cor2, cor2), LARGURA_TELA - 70, ALTURA_TELA - 85, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
			al_flip_display();
		}
		if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				*estado = FIM;
				break;
			case ALLEGRO_KEY_ENTER:
				*estado = MENU;
				break;
			case ALLEGRO_KEY_SPACE:
				*estado = MENU;
				break;
			}
		}
	}
}

void batalha(int* estado) {

	al_detach_audio_stream(ambiente);
	al_detach_audio_stream(menuMusica);
	al_attach_audio_stream_to_mixer(batalhaMusica, al_get_default_mixer());

	int selecionado = 0, fala = 0, efeitoCor = 1, cor = 120, cor2 = 0, atack = 0, efeitoDano = 0, cont = 0, dano[3] = { 0, 0, 0 }, vitoria = 0;
	float vidaProf[3] = { 175, 225, 300 };

	while (*estado == BATALHA)
	{
		while (!al_event_queue_is_empty(fila_eventos))
		{
			ALLEGRO_EVENT evento;
			al_wait_for_event(fila_eventos, &evento);

			if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
				switch (evento.keyboard.keycode)
				{
				case ALLEGRO_KEY_ESCAPE:
					*estado = FIM;
					break;
				case ALLEGRO_KEY_A: case ALLEGRO_KEY_LEFT:
					if (selecionado > 0 && atack) {
						selecionado--;
					}
					break;
				case ALLEGRO_KEY_D: case ALLEGRO_KEY_RIGHT:
					if (selecionado < 3 && atack) {
						selecionado++;
					}
					break;
				case ALLEGRO_KEY_SPACE:
					if (fala == 4) {
						fala++;
						cor2 = 0;
					}
					else if (fala >= 5) {
						player.chaves++;
						if (player.chaves == 4) {
							*estado = CREDITOS;
						}
						else {
							*estado = FORA;
						}
					}
					else if (fala < 3) {
						fala++;
					}
					else if (fala == 3) {
						fala = 1;
						atack = 0;
						if (player.chaves == 1) player.sanidade -= 10;
						else if (player.chaves == 2) player.sanidade -= 25;
						else if (player.chaves == 3) player.sanidade -= 35;
						if (player.sanidade <= 0) {
							gameover(estado);
						}
					}
					atack = 0;
					break;
				case ALLEGRO_KEY_ENTER:
					if (selecionado == 0 && atack) {
						dano[0] = 50; dano[1] = 113; dano[2] = 60;
						vidaProf[0] -= dano[0];
						vidaProf[1] -= dano[1];
						vidaProf[2] -= dano[2];
						efeitoDano = 1;
						fala++;
					}
					else if (selecionado == 1 && atack) {
						dano[0] = 88; dano[1] = 75; dano[2] = 100;
						vidaProf[0] -= dano[0];
						vidaProf[1] -= dano[1];
						vidaProf[2] -= dano[2];
						efeitoDano = 1;
						fala++;
						atack = 0;
					}
					else if (selecionado == 2 && atack) {
						dano[0] = 25; dano[1] = 57; dano[2] = 75;
						vidaProf[0] -= dano[0];
						vidaProf[1] -= dano[1];
						vidaProf[2] -= dano[2];
						efeitoDano = 1;
						fala++;
						atack = 0;
					}
					else if (selecionado == 3 && atack) {
						dano[0] = 10; dano[1] = 45; dano[2] = 150;
						vidaProf[0] -= dano[0];
						vidaProf[1] -= dano[1];
						vidaProf[2] -= dano[2];
						efeitoDano = 1;
						fala++;
						atack = 0;
					}
					if (atack) {
						if (player.chaves == 1) {
							player.IRA += dano[0];
						}
						else if (player.chaves == 2) {
							player.IRA += dano[1];
						}
						else if (player.chaves == 3) {
							player.IRA += dano[2];
						}
					}
					if (vidaProf[0] <= 0 && player.chaves == 1 || vidaProf[1] <= 0 && player.chaves == 2 || vidaProf[2] <= 0 && player.chaves == 3) {
						fala = 4;
						if (player.chaves == 1) {
							player.IRA += 175;
						}
						else if (player.chaves == 2) {
							player.IRA += 225;
						}
						else if (player.chaves == 3) {
							player.IRA += 300;
						}
					}
					break;
				}
			}

			if (evento.type == ALLEGRO_EVENT_TIMER) {
				if (cor2 < 255) {
					cor2++;
				}
				if (efeitoCor) {
					cor += 2;
					if (cor >= 254) {
						efeitoCor = 0;
					}
				}
				else {
					cor -= 2;
					if (cor <= 120) {
						efeitoCor = 1;
					}
				}
				if (cont <= 4 && efeitoDano) {
					cont++;
				}
				else {
					efeitoDano = 0;
					cont = 0;
				}
			}

		}
		al_draw_bitmap_region(fundoBatalha, 0, 0, 1280, 720, 0, 0, 0);
		// PROFESSOR KATU
		if (player.chaves == 1) {
			if (efeitoDano) {
				al_draw_filled_rectangle(928, 160, 928 + ((vidaProf[0] / 175) * 345), 200, al_map_rgb(200, 0, 0));
				al_draw_tinted_bitmap(prof_kato, al_map_rgba(255, 0, 0, 255), 5, 0, 0);
				al_draw_tinted_bitmap(prof_kato, al_map_rgba(255, 0, 0, 255), -5, 0, 0);
			}
			else {
				if (vidaProf[0] >= 0) {
					al_draw_filled_rectangle(928, 160, 928 + ((vidaProf[0] / 175) * 345), 200, al_map_rgb(200, 0, 0));
					al_draw_tinted_bitmap(prof_kato, al_map_rgba(255, 255, 255, 255), 0, 0, 0);
				}
			}

			al_draw_filled_rounded_rectangle(50, 560, 1230, 700, 3, 3, al_map_rgb(17, 17, 28));
			if (fala == 0) {
				al_draw_textf(ralewayreg, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 570, ALLEGRO_ALIGN_CENTRE, "Prof. Katu");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 620, 1000, 22, ALLEGRO_ALIGN_CENTRE, "A tabela verdade e a verdade");
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
			}
			else if (fala == 1) {
				atack = 1;
				al_draw_multiline_textf(ralewayreg, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 575, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Escolha sua habilidade");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), 60 + 285 / 2, 625, 1500, 22, ALLEGRO_ALIGN_CENTRE, "Ver videoaulas");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), 355 + 275 / 2, 625, 1500, 22, ALLEGRO_ALIGN_CENTRE, "Pedir ajuda para um amigo");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), 770, 625, 1500, 22, ALLEGRO_ALIGN_CENTRE, "Esconder apagador");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), 1060, 625, 1500, 22, ALLEGRO_ALIGN_CENTRE, "Confiar nos trabalhos");
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para nao atacar...");
			}
			else if (fala == 2) {
				atack = 0;
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 610, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Katu recebeu %d de dano", dano[0]);
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
			}
			else if (fala == 3) {
				atack = 0;
				al_draw_textf(ralewayreg, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 570, ALLEGRO_ALIGN_CENTRE, "Prof. Katu");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 610, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Katu utilizou super-apagador\nVoce perdeu 10 de Sanidade.");
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
			}
			else if (fala == 4) {
				atack = 0;
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 610, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Voce passou!");
				al_draw_multiline_textf(raleway10, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 640, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Voce pode ir la fora recuperar sanidade.");
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
			}
			else if (fala == 5) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_multiline_textf(raleway20, al_map_rgb(cor2, cor2, cor2), LARGURA_TELA / 2, ALTURA_TELA / 2, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Dia 2");
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
			}

			if (selecionado == 0 && atack) {
				al_draw_rectangle(70, 620, 345, 655, al_map_rgb(255, 255, 255), 1);
			}
			else if (selecionado == 1 && atack) {
				al_draw_rectangle(355, 620, 60 + 2 * 285, 655, al_map_rgb(255, 255, 255), 1);
			}
			else if (selecionado == 2 && atack) {
				al_draw_rectangle(640, 620, 60 + 3 * 285, 655, al_map_rgb(255, 255, 255), 1);
			}
			else if (selecionado == 3 && atack) {
				al_draw_rectangle(925, 620, 60 + 4 * 285, 655, al_map_rgb(255, 255, 255), 1);
			}
		}
		// PROFESSOR DIOGO
		else if (player.chaves == 2) {
			if (efeitoDano) {
				al_draw_filled_rectangle(928, 160, 928 + ((vidaProf[1] / 225) * 345), 200, al_map_rgb(200, 0, 0));
				al_draw_tinted_bitmap(prof_diego, al_map_rgba(255, 0, 0, 255), 5, 0, 0);
				al_draw_tinted_bitmap(prof_diego, al_map_rgba(255, 0, 0, 255), -5, 0, 0);
			}
			else {
				if (vidaProf[1] >= 0) {
					al_draw_filled_rectangle(928, 160, 928 + ((vidaProf[1] / 225) * 345), 200, al_map_rgb(200, 0, 0));
					al_draw_tinted_bitmap(prof_diego, al_map_rgba(255, 255, 255, 255), 0, 0, 0);
				}
			}

			al_draw_filled_rounded_rectangle(50, 560, 1230, 700, 3, 3, al_map_rgb(17, 17, 28));
			if (fala == 0) {
				al_draw_textf(ralewayreg, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 570, ALLEGRO_ALIGN_CENTRE, "Prof. Diogo");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 620, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Problema para o relatorio: Qual a combinacao de ataques eu uso para te derrotar mais rapido?");
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
			}
			else if (fala == 1) {
				atack = 1;
				al_draw_multiline_textf(ralewayreg, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 575, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Escolha sua habilidade");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), 60 + 285 / 2, 625, 1500, 22, ALLEGRO_ALIGN_CENTRE, "Prestar atencao nas aulas");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), 355 + 275 / 2, 625, 1500, 22, ALLEGRO_ALIGN_CENTRE, "Participar");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), 770, 625, 1500, 22, ALLEGRO_ALIGN_CENTRE, "Confiar nos trabalhos");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), 1060, 625, 1500, 22, ALLEGRO_ALIGN_CENTRE, "Confiar na participacao");
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para nao atacar...");
			}
			else if (fala == 2) {
				atack = 0;
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 610, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Diogo recebeu %d de dano", dano[1]);
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
			}
			else if (fala == 3) {
				atack = 0;
				al_draw_textf(ralewayreg, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 570, ALLEGRO_ALIGN_CENTRE, "Prof. Diogo");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 610, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Diogo utilizou pacoca\nVoce perdeu 25 de Sanidade.");
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
			}
			else if (fala == 4) {
				atack = 0;
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 610, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Voce passou!");
				al_draw_multiline_textf(raleway10, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 640, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Voce pode ir la fora recuperar sanidade.");
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
				contFesta = 1;
			}
			else if (fala == 5) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_multiline_textf(raleway20, al_map_rgb(cor2, cor2, cor2), LARGURA_TELA / 2, ALTURA_TELA / 2, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Dia 3");
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
			}

			if (selecionado == 0 && atack) {
				al_draw_rectangle(70, 620, 345, 655, al_map_rgb(255, 255, 255), 1);
			}
			else if (selecionado == 1 && atack) {
				al_draw_rectangle(355, 620, 60 + 2 * 285, 655, al_map_rgb(255, 255, 255), 1);
			}
			else if (selecionado == 2 && atack) {
				al_draw_rectangle(640, 620, 60 + 3 * 285, 655, al_map_rgb(255, 255, 255), 1);
			}
			else if (selecionado == 3 && atack) {
				al_draw_rectangle(925, 620, 60 + 4 * 285, 655, al_map_rgb(255, 255, 255), 1);
			}
		}
		// PROF JOICE
		else if (player.chaves == 3) {
			if (efeitoDano) {
				al_draw_filled_rectangle(928, 160, 928 + ((vidaProf[2] / 300) * 345), 200, al_map_rgb(200, 0, 0));
				al_draw_tinted_bitmap(prof_joice, al_map_rgba(255, 0, 0, 255), 5, 0, 0);
				al_draw_tinted_bitmap(prof_joice, al_map_rgba(255, 0, 0, 255), -5, 0, 0);
			}
			else {
				if (vidaProf[1] >= 0) {
					al_draw_filled_rectangle(928, 160, 928 + ((vidaProf[2] / 300) * 345), 200, al_map_rgb(200, 0, 0));
					al_draw_tinted_bitmap(prof_joice, al_map_rgba(255, 255, 255, 255), 0, 0, 0);
				}
			}

			al_draw_filled_rounded_rectangle(50, 560, 1230, 700, 3, 3, al_map_rgb(17, 17, 28));
			if (fala == 0) {
				al_draw_textf(ralewayreg, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 570, ALLEGRO_ALIGN_CENTRE, "Profa. Joice");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 620, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Alguem me chamou?");
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
			}
			else if (fala == 1) {
				atack = 1;
				al_draw_multiline_textf(ralewayreg, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 575, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Escolha sua habilidade");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), 60 + 285 / 2, 625, 1500, 22, ALLEGRO_ALIGN_CENTRE, "Confiar nos trabalhos");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), 355 + 275 / 2, 625, 1500, 22, ALLEGRO_ALIGN_CENTRE, "Prestar atencao nas aulas");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), 770, 625, 1500, 22, ALLEGRO_ALIGN_CENTRE, "Saber os dias de cada mes");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), 1060, 625, 1500, 22, ALLEGRO_ALIGN_CENTRE, "Fazer listas da Joice");
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para nao atacar...");
			}
			else if (fala == 2) {
				atack = 0;
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 610, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Joice recebeu %d de dano", dano[2]);
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
			}
			else if (fala == 3) {
				atack = 0;
				al_draw_textf(ralewayreg, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 570, ALLEGRO_ALIGN_CENTRE, "Profa. Joice");
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 610, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Joice utilizou lista do URI\nVoce perdeu 35 de Sanidade.");
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
			}
			else if (fala == 4) {
				atack = 0;
				al_draw_multiline_textf(raleway20, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 610, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Voce passou em CAP!");
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
			}
			else if (fala == 5) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_multiline_textf(raleway20, al_map_rgb(cor2, cor2, cor2), LARGURA_TELA / 2, ALTURA_TELA / 2, 1000, 22, ALLEGRO_ALIGN_CENTRE, "Voce passou de semestre!\nParabens!");
				al_draw_textf(raleway10, al_map_rgb(cor, cor, cor), LARGURA_TELA - 70, ALTURA_TELA - 55, ALLEGRO_ALIGN_RIGHT, "Pressione ESPACO para continuar...");
			}

			if (selecionado == 0 && atack) {
				al_draw_rectangle(70, 620, 345, 655, al_map_rgb(255, 255, 255), 1);
			}
			else if (selecionado == 1 && atack) {
				al_draw_rectangle(355, 620, 60 + 2 * 285, 655, al_map_rgb(255, 255, 255), 1);
			}
			else if (selecionado == 2 && atack) {
				al_draw_rectangle(640, 620, 60 + 3 * 285, 655, al_map_rgb(255, 255, 255), 1);
			}
			else if (selecionado == 3 && atack) {
				al_draw_rectangle(925, 620, 60 + 4 * 285, 655, al_map_rgb(255, 255, 255), 1);
			}
		}
		HUD();
		al_flip_display();
	}


}

void jogoFora(int* estado) {

	al_detach_audio_stream(batalhaMusica);
	al_detach_audio_stream(menuMusica);
	al_attach_audio_stream_to_mixer(ambiente, al_get_default_mixer());

	// Variveis
	int cont_frames = 0, coluna_atual = 0, linha_atual = 0, regiao_y_folha = 3 * 1024, regiao_x_folha = 0, w = 0, a = 0, d = 0, s = 0, interacao = 0;
	float dirx = 0, diry = 0;

	// Posiciona de acordo com a situacao (inicio e saida do DC)
	if (situacaoLado == 0) { dirx = -500, diry = -350.0; }
	else if (situacaoLado == 1) { dirx = -1370, diry = -848; }
	mapaObjetos objetos[1];
	objetos[0].x = -600; objetos[0].y = -210; objetos[0].w = -1330; objetos[0].h = -630;

	// Enquanto o estado for FORA (est� do lado de fora do DC)
	while (*estado == FORA) {
		// Enquanto a fila de eventos n�o estiver vazia
		while (!al_is_event_queue_empty(fila_eventos)) {

			// Cria variavel de event e espera por evento
			ALLEGRO_EVENT evento;
			al_wait_for_event(fila_eventos, &evento);

			// Se for um evento timer movimenta a sprite do personagem de acordo com a velocidade
			if (evento.type == ALLEGRO_EVENT_TIMER && w == 1 || s == 1 || a == 1 || d == 1) {
				cont_frames++;
				if (cont_frames >= VELOCIDADE) {
					cont_frames = 0;
					coluna_atual++;
					if (coluna_atual >= 4) {
						coluna_atual = 0;
					}
					regiao_x_folha = coluna_atual * 1024;
				}
				andandoFora(objetos, &diry, &dirx, w, a, s, d, &regiao_y_folha);
			}

			// Se uma tecla for pressionada (sai, interage e define a movimenta��o)
			if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
				switch (evento.keyboard.keycode)
				{
				case ALLEGRO_KEY_ESCAPE:
					*estado = 0;
					break;
				case ALLEGRO_KEY_DOWN: case ALLEGRO_KEY_S:
					s = 1;
					break;
				case ALLEGRO_KEY_UP: case ALLEGRO_KEY_W:
					w = 1;
					break;
				case ALLEGRO_KEY_RIGHT: case ALLEGRO_KEY_D:
					d = 1;
					break;
				case ALLEGRO_KEY_LEFT: case ALLEGRO_KEY_A:
					a = 1;
					break;
				case ALLEGRO_KEY_SPACE:
					if (interacao && dirx <= -992 && dirx >= -1022 && diry <= -1070 && diry >= -1130) {
						primeiraInteracaoNpc(estado);
					}
					else if (interacao && dirx <= -1400 && dirx >= -1440 && diry <= -1004 && diry >= -1028) {
						interacaoFesta(estado);
					}
					break;
				}
			}

			// Se uma tecla levantar reseta a movimenta��o (parar o personagem)
			if (evento.type == ALLEGRO_EVENT_KEY_UP) {
				switch (evento.keyboard.keycode)
				{
				case ALLEGRO_KEY_DOWN: case ALLEGRO_KEY_S:
					s = 0;
					break;
				case ALLEGRO_KEY_UP: case ALLEGRO_KEY_W:
					w = 0;
					break;
				case ALLEGRO_KEY_RIGHT: case ALLEGRO_KEY_D:
					d = 0;
					break;
				case ALLEGRO_KEY_LEFT: case ALLEGRO_KEY_A:
					a = 0;
					break;
				}
			}
		}

		// Reseta posicao na folha quando nao esta se movendo (revendo posibilidade de uso)
		if (w == 0 && s == 0 && a == 0 && d == 0) regiao_x_folha = 0;

		// Fundo fora do DC
		al_draw_tinted_scaled_rotated_bitmap_region(fundo, 0, 0, 2280, 1600, al_map_rgba_f(1, 1, 1, 1), 0, 0, dirx, diry, 1.5, 1.5, 0, 0);

		// Inicializa os dados do NPC e desenha na tela
		al_draw_tinted_scaled_bitmap(folha_spriteNpc, al_map_rgba_f(1, 1, 1, 1), 0, 3 * 1024, 1024, 1024, npc[0].x, npc[0].y, 52, 52, 0);
		if (situacaoLado && player.chaves > 1) {
			al_draw_tinted_scaled_bitmap(folha_spriteNpc2, al_map_rgba_f(1, 1, 1, 1), 0, 1024, 1024, 1024, 1550 + (dirx + 500), 1000 + (diry + 350), 52, 52, 0);
		}
		inicializaNpcs(&dirx, &diry);

		// Escolhe personagem de acordo com sexo
		if (player.sexo) {
			al_draw_tinted_scaled_bitmap(feminino, al_map_rgba_f(1, 1, 1, 1), regiao_x_folha, regiao_y_folha, 1024, 1024, LARGURA_TELA / 2 - 26, ALTURA_TELA / 2 - 26, 52, 52, 0);
		}
		else {
			al_draw_tinted_scaled_bitmap(masculino, al_map_rgba_f(1, 1, 1, 1), regiao_x_folha, regiao_y_folha, 1024, 1024, LARGURA_TELA / 2 - 26, ALTURA_TELA / 2 - 26, 52, 52, 0);
		}

		// Intera��o com o DC
		if (dirx <= -1300 && dirx >= -1425 && diry == -836) {
			interacaoDC(estado, &diry, &w, &a, &d, &s);
		}
		// Texto de intera��o com o NPC inicial
		if (dirx <= -992 && dirx >= -1022 && diry <= -1070 && diry >= -1130) {
			al_draw_multiline_text(ralewayreg, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 550, 800, 20, ALLEGRO_ALIGN_CENTRE, "Pressione ESPACO para interagir");
			interacao = 1; // Permite a intera��o
		}
		else if (dirx <= -1400 && dirx >= -1440 && diry <= -1004 && diry >= -1028 && situacaoLado) {
			al_draw_multiline_text(ralewayreg, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 550, 800, 20, ALLEGRO_ALIGN_CENTRE, "Pressione ESPACO para interagir");
			interacao = 1; // Permite a intera��o
		}
		else {
			interacao = 0; // Bloqueia a intera��o
		}

		HUD(); // HUD com nome, sanidade e IRA
		al_flip_display();
	}
}

void jogoDentro(int* estado) {
	int cont_frames = 0, coluna_atual = 0, linha_atual = 0, regiao_y_folha = 3 * 1024, regiao_x_folha = 0, w = 0, a = 0, d = 0, s = 0, interacao = 0;
	float dirx = -1874, diry = -1326;
	situacaoLado = 1;

	mapaObjetos objetos[1];
	objetos[0].x = -600; objetos[0].y = -210; objetos[0].w = -1330; objetos[0].h = -630;

	while (*estado == DENTRO) {
		while (!al_is_event_queue_empty(fila_eventos)) {
			ALLEGRO_EVENT evento;
			al_wait_for_event(fila_eventos, &evento);
			if (evento.type == ALLEGRO_EVENT_TIMER && w == 1 || s == 1 || a == 1 || d == 1) {
				cont_frames++;
				if (cont_frames >= VELOCIDADE) {
					cont_frames = 0;
					coluna_atual++;
					if (coluna_atual >= 4) {
						coluna_atual = 0;
					}
					regiao_x_folha = coluna_atual * 1024;
				}
				andando(&diry, &dirx, w, a, s, d, &regiao_y_folha);
			}

			if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
				switch (evento.keyboard.keycode)
				{
				case ALLEGRO_KEY_ESCAPE:
					*estado = 0;
					break;
				case ALLEGRO_KEY_DOWN: case ALLEGRO_KEY_S:
					s = 1;
					break;
				case ALLEGRO_KEY_UP: case ALLEGRO_KEY_W:
					w = 1;
					break;
				case ALLEGRO_KEY_RIGHT: case ALLEGRO_KEY_D:
					d = 1;
					break;
				case ALLEGRO_KEY_LEFT: case ALLEGRO_KEY_A:
					a = 1;
					break;
				}
			}

			if (evento.type == ALLEGRO_EVENT_KEY_UP) {
				switch (evento.keyboard.keycode)
				{
				case ALLEGRO_KEY_DOWN: case ALLEGRO_KEY_S:
					s = 0;
					break;
				case ALLEGRO_KEY_UP: case ALLEGRO_KEY_W:
					w = 0;
					break;
				case ALLEGRO_KEY_RIGHT: case ALLEGRO_KEY_D:
					d = 0;
					break;
				case ALLEGRO_KEY_LEFT: case ALLEGRO_KEY_A:
					a = 0;
					break;
				}
			}
		}

		if (w == 0 && s == 0 && a == 0 && d == 0) { regiao_x_folha = 0; }
		al_draw_tinted_scaled_rotated_bitmap_region(dentro, 0, 0, 2280, 1600, al_map_rgba_f(1, 1, 1, 1), 0, 0, dirx, diry, 1.4, 1.4, 0, 0);

		inicializaNpcs(&dirx, &diry);

		if (player.sexo) {
			al_draw_tinted_scaled_bitmap(feminino, al_map_rgba_f(1, 1, 1, 1), regiao_x_folha, regiao_y_folha, 1024, 1024, LARGURA_TELA / 2 - 26, ALTURA_TELA / 2 - 26, 52, 52, 0);
		}
		else {
			al_draw_tinted_scaled_bitmap(masculino, al_map_rgba_f(1, 1, 1, 1), regiao_x_folha, regiao_y_folha, 1024, 1024, LARGURA_TELA / 2 - 26, ALTURA_TELA / 2 - 26, 52, 52, 0);
		}

		if (dirx <= -1820 && dirx >= -1922 && diry == -1356) {
			interacaoDC(estado, &diry, &w, &a, &d, &s);
		}

		// Porta do Kato
		if (dirx <= -1364 && dirx >= -1406 && diry == -276 && player.chaves == 1) {
			interacaoBOSS(estado, &diry, &w, &a, &d, &s, 0);
		}
		else if (dirx <= -1364 && dirx >= -1406 && diry == -276) {
			interacaoBOSS(estado, &diry, &w, &a, &d, &s, 1);
		}
		// Porta do Diego
		if (dirx <= -1058 && dirx >= -1094 && diry == -276 && player.chaves == 2) {
			interacaoBOSS(estado, &diry, &w, &a, &d, &s, 0);
		}
		else if (dirx <= -1058 && dirx >= -1094 && diry == -276) {
			interacaoBOSS(estado, &diry, &w, &a, &d, &s, 1);
		}
		// Porta da Joice
		if (dirx <= -740 && dirx >= -776 && diry == -276 && player.chaves == 3) {
			interacaoBOSS(estado, &diry, &w, &a, &d, &s, 0);
		}
		else if (dirx <= -740 && dirx >= -776 && diry == -276) {
			interacaoBOSS(estado, &diry, &w, &a, &d, &s, 1);
		}

		HUD();
		al_flip_display();
	}
}

void main() {
	int ini = MENU;
	int* estado = &ini;

	if (inicializar()) {
		return 0;
	}

	while (*estado != FIM) {
		if (*estado == MENU) {
			menu(estado);
		}
		else if (*estado == INICIO) {
			inicio(estado);
		}
		else if (*estado == FORA) {
			jogoFora(estado);
		}
		else if (*estado == DENTRO) {
			jogoDentro(estado);
		}
		else if (*estado == BATALHA) {
			batalha(estado);
		}
		else if (*estado == CREDITOS) {
			creditos(estado);
		}
		else if (*estado == RANKING) {
			ranking(estado);
		}
	}
	destroi();
}
