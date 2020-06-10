#include <bits/stdc++.h>
using namespace std;
int current_time, finished_offers, max_waiting_time = -1, OA_waiting_time, max_seq_num, max_cycle_num, entered_offers;
int OA_passes;//количество проходов через ОА
double total_time;//время для подсчета среднего времени пребывания
int OA_downtime;//время простоя
int entrance_waiting_time;//время ожидания
bool queue_reached_empty = 1;



typedef struct link{
    int cycle_number;//количество пройденных циклов
    int arrival_time;//время, в которое пришла заявка
    int sequential_number;//порядковый номер заявки
    struct link *next = NULL;//указатель на следующую заявку
    struct link *prev = NULL;//указатель на предыдущую заявку
}mylist;
mylist *chain_input, *chain_output;//удобно хранить указатели на начало и конец очереди (вход и выход)

void add_to_start(int arrival_time, int sequential_number)//добавление новой заявки в начало очереди
{
    mylist *new_item = new mylist;
    new_item->cycle_number = 0;
    new_item->sequential_number = sequential_number;
    new_item->arrival_time = arrival_time;
    if (chain_output != NULL)//не пуста
    {
        chain_input->next = new_item;
        new_item->prev = chain_input;
        chain_input = new_item;
    }
    else
    {
        chain_input = new_item;
        chain_output = new_item;
    }
    return;
}

int queue_size(mylist *ptr1)
{
    if (ptr1 == NULL)
        return 0;
    return 1 + queue_size(ptr1->next);
}

bool queue_is_empty()
{
    if (queue_size(chain_output) == 0)
        return 1;
    return 0;
}

void new_cycle(int arrival_time)//отправляем заявку на новый цикл
{
    chain_input->next = chain_output;
    chain_output = chain_output->next;
    chain_input = chain_input->next;
    chain_input->next = NULL;
    chain_input->arrival_time = arrival_time;
    chain_input->cycle_number++;
    return;
}

double get_downtime()
{
    int tmp = abs(current_time - OA_waiting_time - entrance_waiting_time);
    return (OA_downtime + tmp) / entered_offers;
}

void remove_from_chain()//заявки с количеством циклов 4 убираем
{
    if (chain_output->next != NULL)
    {
        mylist *tmp = chain_output->next;
        delete chain_output;
        chain_output = tmp;
    }
    else
    {
        chain_output = NULL;
        chain_input = NULL;
    }
    return;
}


int main()
{
    setlocale(LC_ALL, "");
    int t1_value,t2_value;
    cout << "Введите время обработки T1\n";
    cin >> t1_value;
    int t1_current = rand() % t1_value;
    OA_downtime -= t1_current;
    entrance_waiting_time = t1_current;
    cout << "Введите время обработки T2\n";
    cin >> t2_value;
    int t2_current = rand() % t2_value;
    cout << "Номер сотни|Вошедшие заявки|Длина очереди|Номер макс. заявки|Цикл макс. заявки|Макс. время|Среднее время\n";
	while (finished_offers < 1000)
	{
		while (t1_current == 0)//на случай если рандом выдаст непрерывную последовательность
		{
			add_to_start(current_time, entered_offers);
			t1_current = rand() % t1_value;//обновление t1
			entrance_waiting_time += t1_current;
			entered_offers++;
		}
		if (!queue_is_empty() && queue_reached_empty)
        {
            queue_reached_empty = 0;
            t2_current = rand() % t2_value;
            OA_waiting_time += t2_current;
        }

		while ((!queue_reached_empty) && t2_current == 0)//на случай если рандом выдаст непрерывную последовательность
		{
			if (current_time - chain_output->arrival_time > max_waiting_time)
			{
				max_cycle_num = chain_output->cycle_number;
				max_seq_num = chain_output->sequential_number;
				max_waiting_time = current_time - chain_output->arrival_time;
			}
			OA_passes++;
			total_time = (total_time + current_time - chain_output->arrival_time);
			if(chain_output->cycle_number == 3)
			{
				finished_offers++;
				if ((current_time - chain_output->arrival_time) > max_waiting_time)
				{
					max_cycle_num = chain_output->cycle_number + 1;
					cout << max_cycle_num;
                    max_seq_num = chain_output->sequential_number;//обновление максимума
					max_waiting_time = current_time - chain_output->arrival_time;
				}
				remove_from_chain();
				if (finished_offers % 100 == 0)//информация
				    cout << setw(10) << finished_offers / 100 << setw(14) << entered_offers << setw(15) << queue_size(chain_output) << setw(15) << max_seq_num  << setw(20) << max_cycle_num << setw(15) << max_waiting_time << setw(15) << total_time / OA_passes << '\n';
			}
			else if(chain_output->cycle_number < 3)
                new_cycle(current_time);
			if (!queue_is_empty())
			{
				t2_current = rand() % t2_value;
				queue_reached_empty = 0;
				OA_waiting_time += t2_current;
			}
			else
                queue_reached_empty = 1;

		}
		if(entered_offers < 1000)
			t1_current--;
		if (!queue_reached_empty)
        {
			t2_current--;
        }
		current_time++;


	}
	cout << "Общее время моделирования: " << current_time << '\n';
	cout << "Время простоя ОА: " << get_downtime() << '\n';
	cout << "Количество обработанных заявок: "  << OA_passes << '\n';
	cout << "Количество вошедших заявой: " << entered_offers << '\n';
	cout << "Количество вышедших заявок: " << finished_offers << '\n';
    return 0;
}
