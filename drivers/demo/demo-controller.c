#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_demo.h>

MODULE_LICENSE("GPL");

static const struct of_device_id demo_controller_dt_ids[];

static int demo_controller_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	int ret = 0;

	dev_dbg(dev, "%s enter.\n", __func__);

	if (!np) {
		dev_err(dev, "of_node is NULL\n");
		return -EINVAL;
	}

	ret = of_demo_controller_register(np, of_demo_simple_xlate, dev);
	if (ret < 0) {
		dev_err(dev, "%s: register demo controller failed: %d\n", __func__, ret);
		return -EINVAL;
	}

	dev_info(dev, "register demo controller successfully.\n");

	return ret;
}

static int demo_controller_remove(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;

	if (!np) {
		dev_err(dev, "%s of_node is NULL\n", __func__);
		return -EINVAL;
	}

	dev_dbg(dev, "%s enter.\n", __func__);

	of_demo_controller_free(np);
	return 0;
}

static const struct of_device_id demo_controller_dt_ids[] = {
	{
		.compatible = "demo_controller,one",
	}, {
		.compatible = "demo_controller,two",
	}, {
		.compatible = "demo_controller,three",
	}, {
		/* sentinel */
	}
};
MODULE_DEVICE_TABLE(of, demo_controller_dt_ids);

static struct platform_driver demo_controller_driver = {
	.driver		= {
		.name	= "demo-controller",
		.of_match_table = of_match_ptr(demo_controller_dt_ids),
	},
	.probe		= demo_controller_probe,
	.remove		= demo_controller_remove,
};

module_platform_driver(demo_controller_driver);
