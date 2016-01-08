#include <stdio.h>
#include <string.h>
#include <unistd.h>

//struct Unit player[2];
void showAtt();
//---------------------------------------
struct Effect
{
	char effectType[10];
	char target[10];
	double amount;
};

struct Skill
{
	char name[10];
	struct Effect effect;
	double mpCost;
};

struct Unit {
	char name[10];
	double hp, mp, maxHp, maxMp;
	double attackDamage;
	struct Skill skill[5];
};

//全局变量
struct Unit player[2];
struct Effect hammerD;
struct Effect holyLightR;
struct Skill attack;
struct Skill hammer;
struct Skill holyLight;
char dialog[10][9][61];
char dialogNum[10];

//函数
/*添加对话*/void addDialog(int dayIndex, char text[]) {
	sprintf(dialog[dayIndex][dialogNum[dayIndex]], text);
	dialogNum[dayIndex]++;
}

/*运行对话*/void runDialog(int dayIndex) {
	for(int i = 0; i < dialogNum[dayIndex]; i++) {
		printf("%s\n", dialog[dayIndex][i]);
		//printf("%lu\n", strlen(dialog[dayIndex][i]));
		sleep(strlen(dialog[dayIndex][i])/10); 
	}
}

/*伤害*/void damage(struct Unit *target, double amount) {
	target->hp -= amount;
	if (target->hp <= 0) {
		if (target == &player[0])
				//英雄阵亡
				printf("lol u die\n");
		else
				printf("enemy lose\n");
	}
	else showAtt();
}

/*运行效果*/void runEffect(struct Unit *caster, struct Unit *target, struct Effect e) {
	struct Unit *unit;
	//target
	if (strcmp(e.target, "caster") == 0)
	{
		unit = caster;
	}
	else{
		unit = target;
	}
	//effectType
	if(!strcmp(e.effectType,"damage"/*伤害*/)) {
		printf("造成了%.0f点伤害。\n",e.amount);
		damage(unit, e.amount);
	}
	if(!strcmp(e.effectType,"restore"/*恢复*/)) {
		if(unit->hp < unit->maxHp)
			if(unit->hp + e.amount < unit->maxHp)
				unit->hp += e.amount;
			else
				unit->hp = unit->maxHp;
		else{}
		showAtt();
	}
}

/*运行技能*/void runSkill(struct Unit *caster, struct Unit *target, struct Skill skill) {
	if (strcmp(skill.name, "attack") == 0)
		damage(target, caster->attackDamage);
	else 
	{
		printf("%s对%s释放了%s，",caster->name, target->name, skill.name);
		caster->mp -= skill.mpCost;
		runEffect(caster, target, skill.effect);
	}
}
/*显示属性*/void showAtt() {
	printf("我方血量:%.0f  能量:%.0f\n", player[0].hp, player[0].mp);
	printf("敌方血量:%.0f  能量:%.0f\n", player[1].hp, player[1].mp);
	printf("-------------------------------------------------\n");
}

/*玩家行动开始*/void playerStart() {
	int a;
	printf("输入指令:\n1、普通攻击     2、制裁之锤\n3、圣光术\n");
	printf("-------------------------------------------------\n");
	while (scanf("%d", &a) != EOF) {
		if (a >=1 && a <= 3)
			break;
		else {
			printf("无效输入\n");
			continue;
		}
	}
	printf("-------------------------------------------------\n");
	//printf("%d\n", a);
	printf("%s\n", player[0].skill[2].name);
	//runSkill(&player[0], &player[1], player[0].skill[a]);
	switch(a) {
	case 1:
		damage(&player[1], player[0].attackDamage);
		break;
	case 2:
		runSkill(&player[0], &player[1], hammer);
		break;
	case 3:
		runSkill(&player[0], &player[0], holyLight);
		break;
	}
	
}

int main() {
	sprintf(player[0].name, "你");
	memset(dialogNum, 0, sizeof(dialogNum));
	addDialog(0, "321%s123",player[0].name);
	addDialog(0, "一股来自地狱的可怕势力重新回到了人间");
	addDialog(0, "你是唯一能够对抗它的人");
	runDialog(0);
	//主角
	sprintf(player[0].name, "你");
	player[0].hp = 350;
	player[0].maxHp = 500;
	player[0].mp =200;
	player[0].maxMp = 200;
	player[0].attackDamage = 25;
	player[0].skill[1] = attack;
	player[0].skill[2] = hammer;
	player[0].skill[3] = holyLight;

	sprintf(player[1].name, "大水怪");
	player[1].hp = 200;
	player[1].maxHp = 200;

	//制裁之锤效果
	sprintf(hammerD.effectType, "damage");
	sprintf(hammerD.target, "target");
	hammerD.amount = 100;
	//圣光术效果
	sprintf(holyLightR.effectType, "restore");
	sprintf(holyLightR.target, "caster");
	holyLightR.amount = 200;

	sprintf(attack.name, "attack");
	//制裁之锤
	sprintf(hammer.name, "制裁之锤");
	hammer.effect = hammerD;
	hammer.mpCost = 100;
	//圣光术
	sprintf(holyLight.name, "圣光术");
	holyLight.effect = holyLightR;
	holyLight.mpCost = 100;
	showAtt();

	playerStart();
	//runEffect(&player[0], &player[1], hammerD);
	//runEffect(&player[0], &player[1], holyLight);
	//runSkill(&player[0], &player[1], hammer);
	//damage(&player[0], 400);

	return 0;
}
