// prh_tuanyao.h - v0.02 - public domain - swdayu <github.com/swdayu>
// No warranty implied, use at your own risk.

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
////
////   INTERFACE
////
////

#ifndef PRH_IMPL_TUANYAO_INCLUDED_H
#define PRH_IMPL_TUANYAO_INCLUDED_H

#define PRH_FILE_INCLUDE
#define PRH_TIME_INCLUDE
#define PRH_ARRAY_INCLUDE
#include "prh_include.h"

#ifdef __cplusplus
extern "C" {
#endif

定 {
    双 净利润[4];
    双 净资产[4];
} 年度信息;

定 {
    肆 代码: 20, 上交所: 1, 深交所: 1, 主板: 1, 科创板: 1, 创业板: 1, 停牌: 1, 警示: 1, 退市: 1;
    肆 简称;
    肆 现价; // 保持3位小数，0.001元
    肆 昨收;
    肆 涨停;
    肆 跌停;
    肆 板块;
    肆 开始年份;
    肆 结束年份;
    天 上市日期;
    天 退市日期;
    天 退市开始;
    壹 名称[32];
} 标;

定 {
    天 日期;
    肆 现价;
    肆 开盘;
    肆 昨收;
    肆 涨停;
    肆 跌停;
    肆 最高;
    肆 最低;
    肆 分钟成交;
    肆 分时成交;
    肆 分时报价;
    肆 逐笔成交;
    肆 数量[4];
    双 成交量; // 股
    双 成交额; // 元
    双 净利润; // TTM
    双 净资产;
    双 累计后复权因子;
    双 前后复权价比率;
    双 股本总股数;
    双 流通股股数;
    双 自由流通股; // 不包含股东、公司管理层、战略性股东等持有的长期不流通的股份，因此较为真实地反映了市场上流通股份的情况
} 收盘信息;

定 {
    肆 成交量; // 单位100股
    贰 时间戳; // 最多 65535 秒，18个多小时
    贰 超额量;
    肆 开盘价;
    肆 收盘价;
    肆 最高价;
    肆 最低价; // 最大42949672.95元（四千多万元)
    肆 平均价;
} 成交基准; // 28

定 {
    肆 版本: 2, 上涨: 1, 成交量: 29; // 单位100股，最大26843545500股（两百亿股）
    贰 时间差; // 最多 65535 秒，18个多小时
    贰 超额量;
    肆 开价差;
    肆 收价差;
    肆 均价差;
    肆 高价差;
    肆 低价差; // 最多差42949672.95元（四千多万元)
} 成交信息; // 28 * 5KB

定 {
    肆 版本: 2, 上涨: 1, 成交量: 29; // 单位100股，最大26843545500股（两百亿股）
    贰 时间差; // 最多 65535 秒，18个多小时
    贰 开价差;
    贰 收价差;
    贰 均价差;
    贰 高价差;
    贰 低价差; // 最多差655.35元
} 成交第二; // 16 * 5KB

定 {
    肆 版本: 2, 上涨: 1, 成交量: 16, // 单位100股，最大6553500股
       时间差: 13; // 最多8191个时间单位，8191秒可以表示2.27小时
    肆 开价差: 12, //
       收价差: 12, //
       均价差:  8; //
    肆 均价高:  4, // 最多差40.95元
       高价差: 14, // 最多差163.83元
       低价差: 14; // 最多差163.83元
} 成交第三; // 12 * 5KB

定 {
    贰 版本: 2, 上涨: 1, 成交量: 13; // 单位100股，最大819100股
    壹 时间差; // 最多差255个时间单位，时间单位是秒或分
    壹 开价差;
    壹 收价差;
    壹 均价差;
    壹 高价差;
    壹 低价差; // 最多差2.55元
} 成交第四; // 8 * 5KB

prh_static_assert(sizeof(成交信息) == 28 && sizeof(成交信息) % 4 == 0);
prh_static_assert(sizeof(成交第二) == 16 && sizeof(成交第二) % 4 == 0);
prh_static_assert(sizeof(成交第三) == 12 && sizeof(成交第三) % 4 == 0);
prh_static_assert(sizeof(成交第四) ==  8 && sizeof(成交第四) % 4 == 0);

定 {
    正 天数;
    天 交易日[1];
} 年历数据;

定 {
    贰 开始年份;
    贰 结束年份;
    辩 深交所;
} 年度日历;

定 {
    天 基准日期;
    宽 天数;
    辩 深交所;
} 近日日历;

定 {
    壹 *多标;
    天 日期;
    辩 后复权;
} 基础参数;

定 {
    壹 *多标;
    天 开始;
    天 结束;
    辩 后复权;
} 日线参数;

定 {
    壹 *多标;
    分 分钟;
    辩 后复权;
} 分钟参数;

定 {
    壹 *多标;
    天 结束;
    肆 个数;
    辩 后复权;
} 多日参数;

定 {
    壹 *多标;
    分 结束;
    肆 个数;
    辩 后复权;
} 多分参数;

定 {
    壹 *多标;
    戳 开始;
    戳 结束;
    辩 后复权;
} 瞬时参数;

结 单次请求;
定 请求参数 {
    空 (*处理函数)(结 单次请求 *请求, 结 请求参数 *参数);
    并 {
        年度日历 年历;
        近日日历 日历;
        基础参数 基参;
        日线参数 日参;
        多日参数 多日;
        分钟参数 分钟;
        多分参数 多分;
        瞬时参数 瞬时;
    };
} 请求参数;

定 单次请求 {
    请求参数 *开始;
    请求参数 *当前;
    肆 数量;
    辩 错误, 结束;
    空 (*数据处理)(结 单次请求 *请求, 空 *结果, 正 总数, 正 序号);
} 单次请求;

//////////////////////////////////////////////////////////////////////////////
//
// BACKEND INTERFACE
//

#include "tuanyao_backend.h"

正 标的代码转字符串(肆 代码, 壹 *字符串);
空 督取历史数据(戳 *开始时间, 戳 *结束时间, 单次请求 *请求);
空 督年度交易日历(单次请求 *请求, 请求参数 *p);
空 督最近交易日历(单次请求 *请求, 请求参数 *p);
空 督交易日标的信息(单次请求 *请求, 请求参数 *p);
空 督多标历史日线(单次请求 *请求, 请求参数 *p);
空 督多标历史分线(单次请求 *请求, 请求参数 *p);
空 督多标最近日线(单次请求 *请求, 请求参数 *p);
空 督多标最近分线(单次请求 *请求, 请求参数 *p);
空 督多标历史瞬时(单次请求 *请求, 请求参数 *p);
空 策略初始化(空);

//////////////////////////////////////////////////////////////////////////////
//
// PUBLIC INTERFACE
//

空 构请求参数(空);
辩 执行请求(单次请求 *请求, 空 (*数据处理)(单次请求 *请求, 空 *结果, 正 总数, 正 序号));

空 清空请求(空);
空 交易年历(正 开始年份, 正 结束年份, 辩 深交所);
空 交易日历(天 基准日期, 宽 天数, 辩 深交所);
空 基础信息(壹 *标名, 天 交易日);
空 历史日线(壹 *标名, 天 开始, 天 结束, 辩 后复权);
空 历史分线(壹 *标名, 分 分钟, 辩 后复权);
空 瞬时信息(壹 *标名, 戳 *开始, 戳 *结束, 辩 后复权);

壹 *添加标的(肆 代码);
空 后续标的(肆 代码);
正 标的总数(空);

分 交易日下一分钟(分 分钟);
分 交易日上一分钟(分 分钟);

空 更新年度交易日历(肆 年份);

#ifdef __cplusplus
}
#endif

#endif // PRH_IMPL_TUANYAO_INCLUDED_H

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
////
////   IMPLEMENTATION
////
////

#ifdef PRH_TUANYAO_IMPLEMENTATION

// 北冥有鱼，其名为鲲，鲲之大，不知其几千里也。化而为鸟，其名为鹏。
// 鹏之徙于南冥也，水击三千里，抟扶摇而上者九万里，去以六月息者也。
// 抟，徒端反，司马云：抟飞而上也。崔云：柎翼徘徊而上也。
// 扶摇，徐音遥，风名也。司马云：上行风谓之扶摇。
// 五星已出东方，现在世界正在大变，东风压倒西风。
// 欲借东风之势，体察时势之变，解人生之困局成一家之宽裕。
// RIDING THE WIND STRAIGHT UP THE SKY, NINETY THOUSAND LI HIGH.

// 分钟数据，当前分钟的开盘价不一定与上一分钟的收盘价相等。无论是连续竞价还是集
// 合竞价，每分钟的开盘价都是由新一轮买卖力量重新博弈出的成交价，而不是上一分钟
// 成交价的简单延续。价格在每笔交易间跳动，正是市场交易活跃和价格发现的体现。
//
// 在连续竞价交易时段（9:30 ~ 11:30，13:00 ~ 14:57），采用时间优先和价格优先的
// 撮合原则成交。
//  * 上一分钟的收盘价：是上一分钟最后一笔成交的价格
//  * 当前分钟的开盘价：是当前分钟第一笔成交的价格
//
// 在集合竞价交易时段（9:15 ~ 9:25，14:57 ~ 15:00），采用最大成交量的原则成交，
// 即找到一能让成交量最大的价格。这个价格是一段时间内所有买卖申报共同决定的，因
// 此它同样不会机械地等于上一笔交易的价格。

// 当日累计后复权因子。复权因子本质上是一个调整系数，用于消除股票分红、送股、配
// 股等事件对股价造成的“断崖”，将不同时期的价格拉到同一个可比框架下。它的核心价
// 值在于，让股票的历史价格走势变得连续、可比，从而能真实反映其长期的涨跌表现。
//
// 当公司进行分红送配时，股价会除权（比如10元变5元），但这并不代表公司价值腰斩。
// 复权因子就是通过一个连乘的系数，把这些“非市场因素”导致的股价变化给补回来。最
// 常见的应用是计算后复权价格，方法很简单：
//      后复权价格 = 原始价格 × 累计复权因子
//
// 两种主要复权类型。复权因子会从两个方向使用，形成两种常见的价格：
//      复权类型    核心逻辑                                价格基准    回测适用性
//      前复权      保持最新价格不变，向后修正历史价格      当前最新价  不推荐用于策略回测
//      后复权      以上市首日价格为锚，向前累积除权因子    上市首日价  推荐用于策略回测
//
// 对于量化回测来说，后复权是更稳妥的选择。因为前复权的价格基准会随最新交易日变
// 动，导致历史信号对应的价格在不断变化，从而造成回测和实盘的巨大偏差。
//
// 除权。上市证券发生分红、送股、转增股本、配股等情况，交易所会在权益登记日后的
// 除权除息日对该卷作除权除息处理。例如增转后股票数量增加（账号持有的股票会增
// 加），但价值不变，所以股价会降低。除权参考价如下，结果四舍五入至0.01元，送配
// 股与派息率即每股送/配股、派息的比例。
//      (股权登记日收盘价 + 配股价 x 配股率 - 派息率) / (1 + 送股率 + 配股率)
//
// 前复权是以当前的价格（降低后的股价）和股本为基准，测算先前的价格。即将先前的
// 价格缩减，除权前的K线向下平移。前复权保持了技术指标的连续性，均线系统也更顺
// 畅。前复权缩减价格后，之前的股价可能变成负数。前复权参考价：
//      (复权前价格 + 配股价 x 配股率 - 派息率) / (1 + 送股率 + 配股率 + 转增率)
//
// 后复权是以先前的价格与股本为基准，测算当前的价格。即当前的价格增加，除权后的
// K线向上平移。相当于不让账号中持有的股票增加，而是增加股票的价格。后复权能较
// 为精确地计算累计涨幅、区间涨幅、股票真实价值的增加及持股者的真实收益率。例如
// 某时买入，参与全部配送、分红，一直持有到目前的价位的收益率。后复权参考价：
//      复权前价格 x (1 + 送股率 + 配股率 + 转增率) - 配股价 x 配股率 + 派息率
//
// 当天的后复权价格 = 当天的不复权价格（原始股价） × 累计复权因子。这里的关键
// 是，复权因子是从一开始就累积计算的。你可以把后复权价格理解为：以股票上市第一
// 天的价格为“锚点” (即基准)，把所有后续发生的分红、送股、配股等事件的累积影响，
// 都折算到当天的价格上。它展现的是公司“真实价值”的历史累计增长情况。
//
// 定点复权是以某一天为开始点，对这一天之前的每次除权进行连续的前复权处理，对这
// 一天之后的每次除权进行连续的后复权处理。

// ST的全称是 Special Treatment，表示特别处理。这是交易所对财务状况或其他状况出
// 现异常的上市公司股票交易进行特别标记的一种制度，目的是向投资者提示风险。ST主
// 要有两种类型：
//  1.  ST（特别处理）：通常指上市公司连续两年亏损或存在其他财务异常。
//  2.  *ST（退市风险警示）：比ST更严重，通常指公司连续三年亏损或存在重大违法行
//      为，面临退市风险。如果下一年度不能改善，就可能被强制退市。
//
// 根据2026年7月6日起正式实施的最新交易规则，ST股（风险警示股票）的交易规则主要
// 有以下几点核心变化和规定。总的来说，新规实施后，主板ST股的日内波动空间显著增
// 大，其风险特征与普通股的差异在缩小，但其背后公司基本面恶化的本质风险并未改
// 变。在参与交易前，需要充分了解相关制度和公司情况，审慎决策。
//  1.  涨跌幅限制：主板正式告别“5%时代”。这是本次调整最核心的变化。沪深两市主
//      板的ST和*ST股票，其价格涨跌幅限制已从原先的5%调整为10%。调整后，各板块
//      的跌幅限制如下。这一调整旨在统一主板内部规则，提升定价效率，减少因涨跌
//      幅过窄导致的连续“一字”涨停或跌停现象。
//          板块            ST/*ST股票涨跌幅限制
//          沪/深主板       ±10% (与普通股一致)
//          创业板/科创板   ±20% (不变)
//          北交所          ±30% (不变)
//  2.  买入数量限制：单日累计不超50万股。无论哪个板块，投资者通过竞价交易、大
//      宗交易和盘后固定价格交易，当日累计买入的单只风险警示股票，数量不得超过
//      50万股。上市公司回购或5%以上股东按计划增持等情形除外。
//  3.  投资者适当性管理：需签署新版风险揭示书。首次买入深市风险警示股票的普通
//      投资者，需要以纸质或电子方式签署新版《风险揭示书》。券商需通过多种方式
//      向投资者充分揭示主板ST股涨跌幅调整等新风险。
//  4.  此外，还有一条针对极端价格的补充规则：当A股风险警示板股票前收盘价格低于
//      或等于0.1元时，其涨跌幅限制为0.01元（即申报价格最小变动单位）。

static 请求参数 权请求参数[1024];
static 请求参数 *权参数末尾;
static 壹 权全局标名[16*8*1024];
static 壹 *权标名末尾;
static 正 权标名个数;

空 构请求参数(空) {
    权参数末尾 = 权请求参数;
    权标名末尾 = 权全局标名;
}

请求参数 *当前请求(空) {
    if (权参数末尾 >= prh_arrend(权请求参数)) prh_abort_error(权参数末尾 - 权请求参数);
    请求参数 *p = 权参数末尾++;
    memset(p, 0, sizeof(*p));
    return p;
}

空 清空请求(空) {
    权参数末尾 = 权请求参数;
}

肆 请求总数(空) {
    return (肆)(权参数末尾 - 权请求参数);
}

壹 *添加标的(肆 代码) {
    壹 *起始 = 权标名末尾;
    权标名末尾 += 标的代码转字符串(代码, 权标名末尾);
    *权标名末尾 = 0;
    权标名个数 = 1;
    return 起始;
}

空 后续标的(肆 代码) {
    壹 *末 = prh_arrend(权全局标名);
    if (权标名末尾 >= 末 - 16) prh_abort_error(权标名末尾 - 权全局标名);
    *权标名末尾++ = ',';
    权标名末尾 += 标的代码转字符串(代码, 权标名末尾);
    *权标名末尾 = 0;
    权标名个数 += 1;
}

正 标的总数(空) {
    return 权标名个数;
}

空 基础信息(壹 *标名, 天 交易日) {
    请求参数 *p = 当前请求();
    p->处理函数 = 督交易日标的信息;
    p->基参.多标 = 标名;
    p->基参.日期 = 交易日;
}

空 交易年历(正 开始年份, 正 结束年份, 辩 深交所) {
    请求参数 *p = 当前请求();
    p->处理函数 = 督年度交易日历;
    p->年历.开始年份 = (贰)开始年份;
    p->年历.结束年份 = (贰)结束年份;
    p->年历.深交所 = 深交所;
}

空 交易日历(天 基准日期, 宽 天数, 辩 深交所) {
    请求参数 *p = 当前请求();
    p->处理函数 = 督最近交易日历;
    p->日历.基准日期 = 基准日期;
    p->日历.天数 = 天数;
    p->日历.深交所 = 深交所;
}

空 历史日线(壹 *标名, 天 开始, 天 结束, 辩 后复权) {
    请求参数 *p = 当前请求();
    p->处理函数 = 督多标历史日线;
    p->日参.多标 = 标名;
    p->日参.开始 = 开始;
    p->日参.结束 = 结束;
    p->日参.后复权 = 后复权;
}

空 历史分线(壹 *标名, 分 分钟, 辩 后复权) {
    请求参数 *p = 当前请求();
    p->处理函数 = 督多标历史分线;
    p->分钟.多标 = 标名;
    p->分钟.分钟 = 分钟;
    p->分钟.后复权 = 后复权;
}

空 瞬时信息(壹 *标名, 戳 *开始, 戳 *结束, 辩 后复权) {
    请求参数 *p = 当前请求();
    p->处理函数 = 督多标历史瞬时;
    p->瞬时.多标 = 标名;
    p->瞬时.开始 = *开始;
    p->瞬时.结束 = *结束;
    p->瞬时.后复权 = 后复权;
}

分 交易日下一分钟(分 分钟) {
    分 a = 分钟;
    if (a.分 < 59) {
        a.分 += 1;
    } else {
        a.分 = 0;
        prh_real_assert(a.时 < 23);
        a.时 += 1;
    }
    return a;
}

分 交易日上一分钟(分 分钟) {
    分 a = 分钟;
    if (a.分 > 0) {
        a.分 -= 1;
    } else {
        a.分 = 59;
        prh_real_assert(a.时 > 0);
        a.时 -= 1;
    }
    return a;
}

空 内数据空处理(单次请求 *请求, 空 *结果, 正 总数, 正 序号) { /* 空处理 */ }

辩 执行请求(单次请求 *请求, 空 (*数据处理)(单次请求 *请求, 空 *结果, 正 总数, 正 序号)) {
    单次请求 默认请求;
    if (请求 == 空值) 请求 = &默认请求;
    戳 当下 = {0}; prh_local_date(&当下);
    请求->开始 = 权请求参数;
    请求->数量 = 请求总数();
    请求->当前 = 空值;
    请求->错误 = 假;
    请求->结束 = 假;
    请求->数据处理 = 数据处理 ? 数据处理 : 内数据空处理;
    督取历史数据(&当下, &当下, 请求);
    请求->结束 = 真;
    请求->数据处理(请求, 空值, 0, 0);
    return 请求->错误 ? 假 : 真;
}

定 {
    单次请求 头部;
    年历数据 数据;
} 年历请求;

空 内年历数据处理(单次请求 *请求, 空 *结果, 正 总数, 正 序号) {
    年历请求 *p = (年历请求 *)请求;
    天 *交易日 = (天 *)结果;
    if (请求->错误 || 总数 == 0) return;
    if (总数 < 270 && 序号 < 总数 && p->数据.天数 < 270 && 交易日->年 == 请求->当前->年历.开始年份) {
        p->数据.交易日[p->数据.天数++] = *交易日;
    } else {
        prh_eprint("[ERROR] 年历数据 天数 %d %d 序号 %d 年份 %d %d\n",
            (prh_reg)总数,
            (prh_reg)p->数据.天数,
            (prh_reg)序号,
            (prh_reg)请求->当前->年历.开始年份,
            (prh_reg)交易日->年);
        请求->错误 = 真;
    }
}

空 更新年度交易日历(肆 年份)
{
    if (年份 < 1990 || 年份 >= 1990 + 200)
    {
        prh_eprint("[ERROR] 非法年份 %d\n", (prh_reg)年份);
    }

    清空请求();
    交易年历(年份, 年份, 真);
    prh_buffer b = prh_make_buffer(prh_local_alloc(), sizeof(年历请求) + sizeof(天) * 270);
    年历请求 *p = (年历请求 *)b.data;
    p->数据.天数 = 0;

    if (!执行请求(&p->头部, 内年历数据处理) || p->数据.天数 == 0 || p->数据.天数 >= 270)
    {
        prh_eprint("[ERROR] 年历数据 %d 天数 %d\n", (prh_reg)年份, (prh_reg)p->数据.天数);
        goto label_return;
    }

    prh_handle file = prh_open_file_update("a-niandujiaoyirili.txt", false);
    if (file == prh_invalid_handle) goto label_return; // 4 * 200 + 1 + 2700 * 200 = 540801 0x00084081

    prh_r32 offset, year_offset = 4 * (年份 - 1990);
    prh_r32 file_size = prh_file_size_32(file);
    if (file_size < 801)
    {
        prh_byte data[801] = {0}; data[800] = '\n';
        prh_impl_file_write(file, data, 801);
        file_size = 801;
    }
    if (file_size < 801 || (file_size - 801) % 2700 != 0 || (file_size - 801) / 2700 > 200)
    {
        prh_eprint("[ERROR] 非法文件大小 %d\n", (prh_reg)file_size);
        goto label_return;
    }

    prh_impl_file_pread(file, (prh_byte *)&offset, 4, year_offset);
    if (offset == 0)
    {
        offset = (((prh_r32)p->数据.天数) << 20) | file_size;
        prh_impl_file_pwrite(file, (prh_byte *)&offset, 4, year_offset);
        prh_file_seek_end(file);
    }
    else
    {
        prh_r32 count = offset >> 20;
        offset = offset & 0x000fffff;
        if (offset < 801 || (offset - 801) % 2700 != 0 || (offset - 801) / 2700 > 200)
        {
            prh_eprint("[ERROR] 非法数据偏移 %d\n", (prh_reg)offset);
            goto label_return;
        }
        if (count != p->数据.天数)
        {
            prh_eprint("[ERROR] 年份日历 %d 天数不匹配 %d %d\n", (prh_reg)年份, (prh_reg)count, (prh_reg)p->数据.天数);
            goto label_return;
        }
        prh_file_seek_from_begin(file, offset);
    }

    prh_reg i = 0;
    for (; i < p->数据.天数; i += 1)
    {
        天 *交易日 = p->数据.交易日 + i;
        prh_impl_print(file, "%04d-%02d%02d\n", (prh_reg)交易日->年, (prh_reg)交易日->月, (prh_reg)交易日->日);
    }
    for (; i < 270; i += 1)
    {
        prh_impl_print(file, ".........\n");
    }

label_return:
    prh_free_buffer(&b);
    if (file !=  prh_invalid_handle)
    {
        prh_file_close(file);
    }
}

#endif // PRH_TUANYAO_IMPLEMENTATION

// FULL VERSION HISTORY
//
//   0.02 (2026-08-16) with niandujiaoyirili updated
//   0.01 (2026-08-15) initial release
//

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2026 Godelder Brother
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/
